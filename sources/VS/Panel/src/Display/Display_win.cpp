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
static void CreateButton(Control::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title);
/// Создаёт кнопки для меню канала
static void CreateButtonsChannel(Frame *frame, const char *title, int x, int y, Control::E keyChannel, Control::E keyRangeLess, Control::E keyRangeMore, Control::E keyRShiftLess, Control::E keyRShiftMore);


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
	// Рисуем кнопки меню и функциональные

	Control::E keys[2][5] =
	{
		{ Control::GovButton, Control::Indication, Control::Left,  Control::None, Control::None },
		{ Control::Mode,      Control::None,       Control::Right, Control::None, Control::None }
	};

	int x0 = 37;
	int y0 = Frame::HEIGHT + 10;

	int dX = 68;
	int dY = 5;

	int width = 58;
	int height = 25;

	wxSize size = { width, height };

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			Control::E key = keys[j][i];
			CreateButton(key, frame, { x0 + (width + dX) * i, y0 + (height + dY) * j }, size, Control(key).Name());
		}
	}

	// Рисуем кнопки управления

	dX = 5;

	int x00 = 145;

	width = height = 25;
	x0 = x00 + 160;
	y0 = Frame::HEIGHT + 100;

	size.SetWidth(width);
	size.SetHeight(height);

	CreateButton(Control::GovButton, frame, { x0, y0 }, size, "E");
	CreateButton(Control::Left, frame, { x0 - dX - width, y0 }, size, "L");
	CreateButton(Control::Right, frame, { x0 + dX + width, y0 }, size, "R");
	CreateButton(Control::GovLeft, frame, { x0, y0 - height - dY }, size, "U");
	CreateButton(Control::GovRight, frame, { x0, y0 + height + dY }, size, "D");

	// Кнопки времени

	width = 51;
	x0 = x00 + 5;

	y0 = Frame::HEIGHT + 100;

	size.SetWidth(width);

    CreateButton(Control::None, frame, { x0 + width + dY, y0 }, size, "мс");
	CreateButton(Control::None, frame, { x0, y0 }, size, "с");
	y0 += height + dY;
	CreateButton(Control::A, frame, { x0, y0 }, size, "<-");
	CreateButton(Control::B, frame, { x0 + width + dY, y0 }, size, "->");

	int x = x00 + 5 + (2 * width + dX) / 2 - width / 2;

	CreateButton(Control::Test, frame, { x, y0 - height - dY - height - dY }, size, "Развёртка");

	// Кнопки канала A

	int y = Frame::HEIGHT + 200;

	CreateButtonsChannel(frame, "Канал 1", 5 + x00, y, Control::GovButton, Control::Mode, Control::Indication, Control::None, Control::Left);

	// Кнопки канала B

	CreateButtonsChannel(frame, "Канал 1", 120 + x00, y, Control::Right, Control::None, Control::None, Control::None, Control::None);

	CreateButton(Control::A, frame, { 230 + x00, Frame::HEIGHT + 198 }, { 80, 25 }, "ПУСК/СТОП");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void CreateButtonsChannel(Frame *frame, const char *title, int x, int y, Control::E keyChannel, Control::E keyRangeLess, Control::E keyRangeMore, Control::E keyRShiftLess, Control::E keyRShiftMore)
{
    int width = 45;
    int height = 20;

    int dX = 5;
    int dY = 5;

    wxSize size = { width, height };

    CreateButton(keyRangeLess, frame, { x, y }, size, "мВ");
    CreateButton(keyRangeMore, frame, { x, y + height + dY }, size, "В");

    CreateButton(keyRShiftMore, frame, { x + width + 2 * dX, y }, size, "+");
    CreateButton(keyRShiftLess, frame, { x + width + 2 * dX, y + height + dY }, size, "-");

    size.SetHeight(25);
    size.SetWidth(width + width + dX * 2);

    wxPoint pos = { x, y - dY - size.GetHeight() };

    CreateButton(keyChannel, frame, pos, size, title);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetPositionAndSize(Frame *frame)
{
    wxSize size = { Frame::WIDTH + 9, Frame::HEIGHT + 320 };

    frame->SetSize(size);
    frame->SetMinSize(size);
    frame->SetMaxSize(size);

    wxRect rect = GetMaxDisplay();

    frame->SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void CreateButton(Control::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title)
{
    wxButton *button = new wxButton(frame, (wxWindowID)key, title, pos, size);

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
