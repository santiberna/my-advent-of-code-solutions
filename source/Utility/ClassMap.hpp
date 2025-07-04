#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vcruntime_typeinfo.h>

namespace detail
{
using UnderlyingContainer = std::unordered_map<std::type_index, std::shared_ptr<void>>;
}

class ClassMap : private detail::UnderlyingContainer
{
    using Base = detail::UnderlyingContainer;

public:
    template <typename T, typename... Args>
    void emplace(Args&&... args)
    {
        Base::emplace(typeid(T), std::forward<Args>(args)...);
    }

    template <typename T>
    T* find()
    {
        if (auto it = Base::find(typeid(T)); it != Base::end())
        {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }
};