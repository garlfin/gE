#include "Windowing/DemoWindow.h"

int main()
{
    if(gE::Init(4, 6)) return gE::FAIL;

    gE::Result result;
    gE::DemoWindow window("Test DemoWindow", 1024, 1024, &result);

    if(result) return result;

    window.Run();

    gE::Destroy();
}