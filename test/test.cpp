#include "sfv_net.hpp"

enum class CustomMsgT : uint32_t
{
    TestElement
};

int main()
{
    sfv::net::message<CustomMsgT> msg;
    msg.header.id = CustomMsgT::TestElement;

    int a = 1;
    char b = 'b';
    bool c = false;

    struct
    {
        float x;
        float y;
    } d[5];

    msg << a << b << c << d;

    a = 2;
    b = 'g';
    c = true;

    msg >> d >> c >> b >> a;

    std::cout << a << " " << b << " " << c << std::endl;
    return 0;
}