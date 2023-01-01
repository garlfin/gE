#include "Windowing/DemoWindow.h"

int main()
{
    if(gE::Init(4, 6, false)) return gE::FAIL;

    gE::Result result;
    gE::DemoWindow window("Test DemoWindow", 1280, 720, &result);

    if(result) return result;

    window.Run();

    gE::Destroy();
}