#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include <libevdevxx/evdevxx.hpp>


using std::cout;
using std::endl;
using std::flush;

using namespace std::literals;


int main()
{
    using evdev::Code;

    evdev::Device dev;

    dev.name("Fake Mouse");

    dev.enable_rel(Code{REL_X});
    dev.enable_rel(Code{REL_Y});
    // needs at least one button to be recognized as a mouse
    dev.enable_key(Code{BTN_LEFT});

    evdev::Uinput udev{dev};
    cout << "Created uinput device at "
         << udev.devnode().value()
         << endl;
    cout << "Doing a circle... " << flush;

    const float radius = 10;
    for (int i = 0; i < 100; ++i) {

        float angle = i/100.0f * 2 * M_PI;
        int x = static_cast<int>(radius * std::cos(angle));
        int y = static_cast<int>(radius * std::sin(angle));

        udev.rel(Code{REL_X}, x);
        udev.rel(Code{REL_Y}, y);
        udev.flush();

        std::this_thread::sleep_for(16ms);
    }

    cout << "done." << endl;
}
