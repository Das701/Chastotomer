#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Font/Font.h"
#include "GUI/GovernorGUI.h"
#include "Keyboard/Keyboard.h"
#include "Utils/Math.h"

#pragma warning(push, 0)
#undef CRC
#include "Application_win.h"

#define uint    unsigned int
#define int8    signed char
#define uint8   unsigned char
#define int16   signed short
#define uint16  unsigned short
#define uint    unsigned int
#define uchar   unsigned char
#define pString const char * const

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


using namespace Primitives;


// Здесь хранятся указатели на кнопки
static wxButton *buttons[Control::Count] = { nullptr };

static GovernorGUI *governor = nullptr;

static bool needStartTimerLong = false;
static bool needStopTimerLong = false;

// Здесь имя нажатой кнопки
static Control::E pressedKey = Control::None;

// Контекст рисования
wxMemoryDC memDC;

static wxBitmap bitmap(Display::WIDTH, Display::HEIGHT);


// Создаёт окно приложения. Возвращает хэндл виджета для отрисовки
static void CreateFrame();
// Установить размер и оптимальную позицию для окна приложения
static void SetPositionAndSize(Frame *frame);
// Получить разрешение максимального имеющегося в системе монитора
static wxRect GetMaxDisplay();
// Создаёт все кнопки
static void CreateButtons(Frame *frame);
// Создаёт одну кнопку
static void CreateButton(Control::E key, Frame *frame, const wxPoint &pos, const wxSize &size);


class Screen : public wxPanel
{
public:
    Screen(wxWindow *parent) : wxPanel(parent, 320)
    {
        SetMinSize({ Display::WIDTH, Display::HEIGHT });
        SetDoubleBuffered(true);
        Bind(wxEVT_PAINT, &Screen::OnPaint, this);
    }

    void OnPaint(wxPaintEvent &)
    {
        wxPaintDC dc(this);
        dc.DrawBitmap(bitmap, 0, 0);
    }
};


static Screen *screen = nullptr;


static wxBitmap *backgroundBMP = nullptr;

void Display::InitHardware()
{
    CreateFrame();

    Font::Set(TypeFont::GOSTAU16BOLD);

    Font::SetSpacing(2);

    static unsigned char buffer[Display::WIDTH * Display::HEIGHT * 3];

    unsigned char *pointer = buffer;

    Color color1 = Color::GREEN_10;
    Color color2 = Color::GREEN_25;

    for (int i = 0; i < Display::WIDTH * Display::HEIGHT; i += 2)
    {
        *pointer++ = RED_FROM_COLOR(COLOR(color1.value));
        *pointer++ = GREEN_FROM_COLOR(COLOR(color1.value));
        *pointer++ = BLUE_FROM_COLOR(COLOR(color1.value));

        *pointer++ = RED_FROM_COLOR(COLOR(color2.value));
        *pointer++ = GREEN_FROM_COLOR(COLOR(color2.value));
        *pointer++ = BLUE_FROM_COLOR(COLOR(color2.value));

        if (i % Display::WIDTH == 0)
        {
            Math::Swap(color1, color2);
        }
    }

    wxImage image;
    image.Create(Display::WIDTH, Display::HEIGHT, buffer, true);

    static wxBitmap bmp(image);

    backgroundBMP = &bmp;
}


void Display::BeginScene()
{
    memDC.SelectObject(bitmap);
    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    memDC.SetBrush(brush);

    memDC.DrawBitmap(*backgroundBMP, 0, 0);
}


void Display::EndScene()
{
    memDC.SelectObject(wxNullBitmap);
    screen->Refresh();
}


void Frame::OnTimerLong(wxTimerEvent&)
{
}


void Frame::HandlerEvents()
{
}


static void CreateFrame()
{
	Frame *frame = new Frame("");

	SetPositionAndSize(frame);

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    screen = new Screen(frame);

    sizer->Add(screen);

	frame->SetSizer(sizer);

	CreateButtons(frame);

	frame->Show(true);
}


static void CreateButtons(Frame *frame)
{
    int x0 = 500;
    int y0 = 15;

    int dX = 10;
    int dY = 5;

    int width = 65;
    int height = 25;

    wxSize size(width, height);

    Control::E keys1[3] = { Control::Channels, Control::Mode, Control::Indication};

    Control::E keys2[3] = { Control::Service, Control::Test, Control::Auto};

    Control::E keys3[3] = { Control::Enter, Control::Left, Control::Right};

    for (int i = 0; i < 3; i++)
    {
        CreateButton(keys1[i], frame, { x0 + (width + dX) * i, y0 }, size);
        CreateButton(keys2[i], frame, { x0 + (width + dX) * i, y0 + height + dY + 10 }, size);
        CreateButton(keys3[i], frame, { x0 + (width + dX) * i, y0 + (height + dY) * 2 + 10 }, size);
    }

    governor = new GovernorGUI(frame, { 700, 150 });
}


static void SetPositionAndSize(Frame *frame)
{
    wxSize size = { Display::WIDTH + 330, Display::HEIGHT };

    frame->SetClientSize(size);
    frame->SetMinClientSize(size);
    frame->SetMaxClientSize(size);

    wxRect rect = GetMaxDisplay();

    frame->SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}


static void CreateButton(Control::E key, Frame *frame, const wxPoint &pos, const wxSize &size)
{
    if (key == Control::None)
    {
        return;
    }

    wxButton *button = new wxButton(frame, (wxWindowID)key, Control(key).Name().c_str(), pos, size);

    button->Connect((wxWindowID)key, wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect((wxWindowID)key, wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
}


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
