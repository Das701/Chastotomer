#include "defines.h"
#include "HAL.h"
#include "stm32f4xx_hal.h"
#include "Keyboard.h"

static int pulseCountRight = 0;
static int pulseCountLeft = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TIME_UPDATE_KEYBOARD 2   // Время между опросами клавиатуры
#define NUM_RL 4
#define NUM_SL 4

#define PIN_SL0  GPIO_PIN_9
#define PORT_SL0 GPIOB

#define PIN_SL1  GPIO_PIN_8
#define PORT_SL1 GPIOB

#define PIN_SL2  GPIO_PIN_7
#define PORT_SL2 GPIOB

#define PIN_SL3  GPIO_PIN_6
#define PORT_SL3 GPIOB

#define PIN_RL0  GPIO_PIN_5
#define PORT_RL0 GPIOB

#define PIN_RL1  GPIO_PIN_4
#define PORT_RL1 GPIOB

#define PIN_RL2  GPIO_PIN_2
#define PORT_RL2 GPIOD

#define PIN_RL3  GPIO_PIN_15
#define PORT_RL3 GPIOA

#define PIN_ENC1  GPIO_PIN_11
#define PORT_ENC1 GPIOC

#define PIN_ENC2  GPIO_PIN_10
#define PORT_ENC2 GPIOC


static TIM_HandleTypeDef handleTIM4;


static Control::E controls[NUM_SL][NUM_RL] =
{
    {Control::Esc,          Control::Test,  Control::Auto,          Control::GovButton},
    {Control::Channels,     Control::Mode,  Control::Indication,    Control::None},
    {Control::Enter,        Control::Left,  Control::None,          Control::None},
    {Control::Right,        Control::None,  Control::None,          Control::None},
};

/// Очередь сообщений - здесь все события органов управления
#define MAX_ACTIONS 100
static Control actions[MAX_ACTIONS];
/// Количество уже имеющихся сообщений
static int numActions = 0;

/// Установленное в true значение означает, что сохранять куда-либо информацию о её состоянии нельзя до отпускания (чтобы не было ложных срабатываний типа Long
static bool alreadyLong[NUM_RL][NUM_SL];
/// При обнаружении нажатия кнопки сюда записывается время нажатия
static uint timePress[NUM_SL][NUM_RL];

/// Время последнего автонажатия нопки
volatile uint prevRepeat = 0;
volatile uint prevPause  = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Установить состояние пина
static void Set_SL(int, int);
/// Установить состояние всех пинов в одно положение
static void Set_All_SL(int);
/// Возвращает состояние пина rl
static int Read_RL(int rl);
/// Инициализировать пины
static void InitPins();
/// Инициализировать таймер для периодического опроса клавиатуры
static void InitTimer();
/// Функция, периодически вызываемая по прерыванию таймера
static void Update();
/// Добавить действие в буфер
static void AddAction(Control control, Control::Action::E action);
/// Обработка ручки
static void DetectRegulator();

#define BUTTON_IS_PRESS(state) ((state) == 0)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Update()
{
    uint time = HAL_GetTick();

    Set_All_SL(1);

    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        Set_SL(sl, 0);

        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            int state = Read_RL(rl);

            if(state == 0)
            {
                state = state;
            }
            else
            {
                state = state;
            }

            Control control = controls[sl][rl];

            if (control.value != Control::None)
            {
                if (timePress[sl][rl])                      // Если клавиша находится в нажатом положении
                {
                    if (time - timePress[sl][rl] > 100)     // Если прошло более 100 мс с момента нажатия
                    {
                        if (!BUTTON_IS_PRESS(state))        // Если сейчас кнопка находится в отжатом состояини
                        {
                            timePress[sl][rl] = 0;
                            if (!alreadyLong[sl][rl])
                            {
                                AddAction(controls[sl][rl], Control::Action::Release);
                            }
                            alreadyLong[sl][rl] = false;
                            prevRepeat = 0;
                        }
                        else if (time - timePress[sl][rl] > 500 && !alreadyLong[sl][rl])
                        {
                            AddAction(controls[sl][rl], Control::Action::Long);
                            alreadyLong[sl][rl] = true;
                        }
                        else
                        {
                            // остальное не обрабатываем
                        }
                    }
                }
                else if (BUTTON_IS_PRESS(state) && !alreadyLong[sl][rl])
                {
                    timePress[sl][rl] = time;
                    AddAction(controls[sl][rl], Control::Action::Press);
                    prevRepeat = 0;
                }
                else
                {
                    // остальное не обрабатываем
                }
            }
        }

        Set_All_SL(1);
    }

    DetectRegulator();
    Set_All_SL(1);
}


static void DetectRegulator()
{  
// Детектируем поворот
    static bool prevStatesIsOne = false;

    bool stateLeft = (HAL_GPIO_ReadPin(PORT_ENC1, PIN_ENC1) == GPIO_PIN_SET);
    bool stateRight = (HAL_GPIO_ReadPin(PORT_ENC2, PIN_ENC2) == GPIO_PIN_SET);

    if (stateLeft && stateRight)
    {
        prevStatesIsOne = true;
    }
    else if (prevStatesIsOne && stateLeft && !stateRight)
    {
        prevStatesIsOne = false;
        pulseCountLeft++;
        if(pulseCountLeft == 2)
        {
            AddAction(Control::GovLeft, Control::Action::Press);
            pulseCountLeft = 0;
        }
    }
    else if (prevStatesIsOne && !stateLeft && stateRight)
    {
        prevStatesIsOne = false;
        pulseCountRight++;
        if(pulseCountRight == 2)
        {
            AddAction(Control::GovRight, Control::Action::Press);
            pulseCountRight = 0;
        }
    }
    else
    {
        // здесь ничего
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::Init()
{
    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            timePress[sl][rl] = 0;
        }
    }

    InitPins();
    InitTimer();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Set_All_SL(int st)
{
    for(int i = 0; i < 4; i++)
    {
        Set_SL(i, st);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Set_SL(int bus, int st)
{
    static const GPIO_TypeDef *ports[4]= {PORT_SL0, PORT_SL1, PORT_SL2, PORT_SL3};
    static const uint16 pins[4] =        {PIN_SL0,  PIN_SL1,  PIN_SL2,  PIN_SL3};
    static const GPIO_PinState state [2] = {GPIO_PIN_RESET, GPIO_PIN_SET};
    
    HAL_GPIO_WritePin((GPIO_TypeDef *)ports[bus], pins[bus], state[st]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Read_RL(int rl)
{
    static const GPIO_TypeDef *ports[4] = { PORT_RL0, PORT_RL1, PORT_RL2, PORT_RL3};
    static const uint16 pins[4] =         { PIN_RL0,  PIN_RL1,  PIN_RL2, PIN_RL3};

    return HAL_GPIO_ReadPin((GPIO_TypeDef *)ports[rl], pins[rl]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void InitPins()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    GPIO_InitTypeDef is =
    {
        PIN_SL0 | PIN_SL1 | PIN_SL2 | PIN_SL3 | PIN_RL0 | PIN_RL1,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };
    HAL_GPIO_Init(GPIOB, &is);

    is.Pin = PIN_RL0 | PIN_RL1;
    is.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &is);

    is.Pin = PIN_RL2;
    HAL_GPIO_Init(GPIOD, &is);
    
    is.Pin = PIN_RL3;
    HAL_GPIO_Init(GPIOA, &is);
    
    is.Pin = PIN_ENC1 | PIN_ENC2;
    HAL_GPIO_Init(GPIOC, &is);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void InitTimer()
{
    __HAL_RCC_TIM4_CLK_ENABLE();

    // Инициализируем таймер, по прерываниям которого будем опрашивать клавиатуру
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);

    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    handleTIM4.Instance = TIM4;
    handleTIM4.Init.Period = TIME_UPDATE_KEYBOARD * 10 - 1;
    handleTIM4.Init.Prescaler = (uint)((SystemCoreClock / 2) / 10000) - 1;
    handleTIM4.Init.ClockDivision = 0;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&handleTIM4) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_TIM_Base_Start_IT(&handleTIM4) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    Set_All_SL(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void AddAction(Control control, Control::Action::E action)
{
    if (action != Control::Action::Press)
    {
        return;
    }

    control.action = action;
    actions[numActions++] = control;
    
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Keyboard::Empty()
{
    return numActions == 0;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control Keyboard::NextControl()
{
    if (Empty())
    {
        return Control();
    }

    Control result = actions[0];

    for (int i = 1; i < numActions; i++)
    {
        actions[i - 1] = actions[i];
    }

    --numActions;
    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char *Control::Name()
{
    static const char *names[] =
    {
        "Ручка нажать",
        "РЕЖИМ",
        "ИНДИКАЦИЯ",
        "3",
        "<-",
        "->",
        "6",
        "7",
        "8",
        "9",
        "A",
        "B",
        "C",
        "D",
        "F5",
        "ESC",
        "Left",
        "Right",
        "Ручка влево",
        "Ручка вправо",
        "Channel",
        "ТЕСТ",
        "АВТО",
        "NULL"
    };

    return names[value];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

    void TIM4_IRQHandler();

    void TIM4_IRQHandler()
    {
        HAL_TIM_IRQHandler(&handleTIM4);
    }

#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //-V2009
{
    if (htim == &handleTIM4)
    {
        Update();
    }
}
