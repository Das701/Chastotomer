#include "defines.h"
#include "Display/Display.h"
#include "Keyboard/Keyboard.h"

#pragma warning(push)
#pragma warning(disable:4365 4459 4668)
#undef CRC
#include "Application.h"

#define uint    unsigned int
#define int8    signed char
#define uint8   unsigned char
#define int16   signed short
#define uint16  unsigned short
#define uint    unsigned int
#define uchar   unsigned char
#define pString const char * const

#include <SDL.h>
#include <wx/display.h>
#pragma warning(pop)

#undef uint   
#undef int8   
#undef uint8  
#undef int16  
#undef uint16 
#undef uint   
#undef uchar  
#undef pString


SDL_Renderer *renderer = nullptr;
static SDL_Window *window = nullptr;
static SDL_Texture *texture = nullptr;

/// Здесь хранятся указатели на кнопки
static wxButton *buttons[Control::E::Count] = { nullptr };
/// Цвета
static uint colors[256];

static bool needStartTimerLong = false;
static bool needStopTimerLong = false;
/// Здесь имя нажатой кнопки
static Control::E pressedKey = Control::None;


/// Создаёт окно приложения. Возвращает хэндл виджета для отрисовки
static HANDLE CreateFrame();
/// Установить размер и оптимальную позицию для окна приложения
static void SetPositionAndSize(Frame *frame);
/// Получить разрешение максимального имеющегося в системе монитора
static wxRect GetMaxDisplay();
/// Создаёт все кнопки
static void CreateButtons(Frame *frame);
/// Создаёт одну кнопку
static void CreateButton(Control::E key, Frame *frame, const wxPoint &pos, const wxSize &size);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
	HANDLE handle = CreateFrame();

	window = SDL_CreateWindowFrom(handle);

	if (window == nullptr)
	{
		std::cout << "SDL_CreateWindowFrom() Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		std::cout << "Create SDL window is ok" << std::endl;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::BeginScene(Color color)
{
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_RENDERER_ACCELERATED, Display::WIDTH, Display::HEIGHT);

	SDL_SetRenderTarget(renderer, texture);
	color.SetAsCurrent();
	SDL_RenderClear(renderer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::EndScene()
{
	SDL_SetRenderTarget(renderer, NULL);

	SDL_Rect rect = { 0, 0, Frame::WIDTH, Frame::HEIGHT };

	if (texture)
	{
		SDL_RenderCopy(renderer, texture, NULL, &rect); //-V2001
	}

	SDL_RenderPresent(renderer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Display::GetPixel(int, int)
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::OnTimerLong(wxTimerEvent&)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::HandlerEvents()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static HANDLE CreateFrame()
{
	Frame *frame = new Frame("");

	SetPositionAndSize(frame);

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton *button = new wxButton(frame, wxID_ANY, "", { 10, 10 }, { Frame::WIDTH, Frame::HEIGHT });
	button->SetMaxSize({ Frame::WIDTH, Frame::HEIGHT });

	sizer->Add(button);

	frame->SetSizer(sizer);

	CreateButtons(frame);

	frame->Show(true);

	return button->GetHandle();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void CreateButtons(Frame *frame)
{
    int x0 = 20;
    int y0 = Frame::HEIGHT + 15;

    int dX = 10;
    int dY = 5;

    int width = 65;
    int height = 25;

    wxSize size(width, height);

    // Кнопки каналов
    Control::E keysCh[4] = { Control::A, Control::B, Control::C, Control::D };

    // Кнопки режимов
    Control::E keysM[4] = { Control::Mode, Control::Indication, Control::Test, Control::Auto };

    // Управляющие кнопки
    Control::E keysC[4] = { Control::Left, Control::Esc, Control::Right, Control::None };

    // Кнопки регулятора
    Control::E keysGov[4] = { Control::GovLeft, Control::GovButton, Control::GovRight, Control::None };

    for (int i = 0; i < 4; i++)
    {
        CreateButton(keysCh[i], frame, { x0 + (width + dX) * i, y0 }, size);
        CreateButton(keysM[i], frame, { x0 + (width + dX) * i, y0 + height + dY }, size);
        CreateButton(keysC[i], frame, { x0 + (width + dX) * i, y0 + (height + dY) * 2 + 10 }, size);
        CreateButton(keysGov[i], frame, { x0 + (width + dX) * i, y0 + (height + dY) * 3 + 20 }, size);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetPositionAndSize(Frame *frame)
{
    wxSize size = { Frame::WIDTH + 9, Frame::HEIGHT + 230 };

    frame->SetSize(size);
    frame->SetMinSize(size);
    frame->SetMaxSize(size);

    wxRect rect = GetMaxDisplay();

    frame->SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void CreateButton(Control::E key, Frame *frame, const wxPoint &pos, const wxSize &size)
{
    if (key == Control::None)
    {
        return;
    }

    wxButton *button = new wxButton(frame, (wxWindowID)key, Control(key).Name(), pos, size);

    button->Connect((wxWindowID)key, wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect((wxWindowID)key, wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static wxRect GetMaxDisplay()
{
    wxRect result = { 0, 0, 0, 0 };

    for (uint i = 0; i < wxDisplay::GetCount(); i++)
    {
        wxDisplay display(i);

        wxRect rect = display.GetClientArea();
        if (rect.width > result.width)
        {
            result.width = rect.width;
            result.height = rect.height;
        }
    }

    return result;
}


#include "Primitives_win.h"

#include "Colors_win.h"
