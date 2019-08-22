#include "defines.h"
#include "HAL.h"
#include "stm32f4xx_hal.h"
#include "Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TIME_UPDATE_KEYBOARD 2   ///< Время между опросами клавиатуры
#define NUM_RL 5
#define NUM_SL 5


static TIM_HandleTypeDef handleTIM4;


static Control::E controls[NUM_SL][NUM_RL] =
{
    {Control::_0,   Control::_1,    Control::_2,     Control::_3,  Control::_4},
    {Control::_5,   Control::_6,    Control::_7,     Control::_8,  Control::_9},
    {Control::_F1,  Control::_F2,   Control::_F3,    Control::_F4, Control::_F5},
    {Control::_ESC, Control::_Left, Control::_Right, Control::_0,  Control::_0},
    {Control::_0,   Control::_1,    Control::_2,     Control::_3,  Control::_4}
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
static uint prevRepeat = 0;
static uint prevPause  = 0;


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

#define BUTTON_IS_PRESS(state) ((state) == 0)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Update()
{
    uint time = HAL_GetTick();

    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        Set_SL(sl, 0);

        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            int state = Read_RL(rl);

            Control control = controls[sl][rl];

            if (control.value != Control::_NULL)
            {
                if (timePress[rl][sl])                      // Если клавиша находится в нажатом положении
                {
                    if (time - timePress[rl][sl] > 100)     // Если прошло более 100 мс с момента нажатия
                    {
                        if (!BUTTON_IS_PRESS(state))        // Если сейчас кнопка находится в отжатом состояини
                        {
                            timePress[rl][sl] = 0;
                            if (!alreadyLong[rl][sl])
                            {
                                AddAction(controls[rl][sl], Control::Action::Release);
                            }
                            alreadyLong[rl][sl] = false;
                            prevRepeat = 0;
                        }
                        else if (time - timePress[rl][sl] > 500 && !alreadyLong[rl][sl])
                        {
                            AddAction(controls[rl][sl], Control::Action::Long);
                            alreadyLong[rl][sl] = true;
                        }
                        else
                        {
                            // остальное не обрабатываем
                        }
                    }
                }
                else if (BUTTON_IS_PRESS(state) && !alreadyLong[rl][sl])
                {
                    timePress[rl][sl] = time;
                    AddAction(controls[rl][sl], Control::Action::Press);
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

    Set_All_SL(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::Init()
{
    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        for (int rl = 0; rl < NUM_SL; ++rl)
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
    for(int i = 0; i < 5; i++)
    {
        Set_SL(i, st);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Set_SL(int bus, int st)
{
    static const GPIO_TypeDef *ports[5]= {GPIOB, GPIOB, GPIOB, GPIOB, GPIOD};
    static const uint16 pins[5] = {GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15,GPIO_PIN_8};
    static const GPIO_PinState state [2] = {GPIO_PIN_RESET, GPIO_PIN_SET};
    
    HAL_GPIO_WritePin((GPIO_TypeDef *)ports[bus], pins[bus], state[st]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Read_RL(int rl)
{
    static const GPIO_TypeDef *ports[5] = { GPIOA, GPIOA, GPIOA, GPIOD, GPIOD };
    static const uint16 pins[5] = { GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_13,GPIO_PIN_12 };

    return HAL_GPIO_ReadPin((GPIO_TypeDef *)ports[rl], pins[rl]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void InitPins()
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
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
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "ESC",
        "Left",
        "Right",
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
