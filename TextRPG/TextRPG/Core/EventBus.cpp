#include "EventBus.h"

void EventBus::Unsubscribe(size_t id)
{
    //해당 코드에서 오류 발생시 컴파일러의 C++버전을 C++17이상으로 올려주세요.
    for (auto& [type, list] : subscribers)
    {
        list.erase(
            std::remove_if(
                list.begin(),
                list.end(),
                [&](auto& s)
                {
                    return s.id == id;
                }),
            list.end());
    }
}