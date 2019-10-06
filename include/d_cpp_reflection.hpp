#pragma once
#include <string>
#include <functional>
#include <sstream>

namespace entity {
class Song;
}

namespace d_reflection {
enum class FieldTypeT : uint8_t { STRING, UINT64, UINT32, UNKNOWN };

template <class T>
typename std::enable_if<
    std::is_same<decltype(std::declval<const T &>().to_string()), std::string>::value,
    std::string>::type
my_to_string(const T &t) {
    return t.to_string();
}

template <class T>
typename std::enable_if<
    std::is_same<decltype(std::to_string(std::declval<T &>())), std::string>::value,
    std::string>::type
my_to_string(const T &t) {
    return std::to_string(t);
}

template <typename container_class>
struct ReflectFieldInfoBase {
    virtual ~ReflectFieldInfoBase() = default;
    std::string name_;

    virtual std::string value_as_string(const container_class *) const = 0;
    virtual FieldTypeT field_type() const = 0;
    virtual void set_value_from_str(container_class *, const std::string &value) = 0;

    explicit ReflectFieldInfoBase(std::string name) : name_(std::move(name)) {}

    // const std::string &name() { return name_; }
};

template <typename container_class, typename T_field>
struct ReflectFieldInfo : public ReflectFieldInfoBase<container_class> {
    std::string value_as_string(
        const container_class *const reflected_obj_ptr) const override {
        if constexpr (std::is_same<std::string, T_field>::value) {
            return f_getter_(reflected_obj_ptr);
        } else {
            return my_to_string(f_getter_(reflected_obj_ptr));
        }
    }

    void set_value_from_str(container_class *const reflected_obj,
                            const std::string &value) override {
        if constexpr (std::is_same<std::string, T_field>::value) {
            f_setter_(reflected_obj, value);
        } else {
            std::istringstream ss(value);
            T_field something;
            ss >> something;
            f_setter_(reflected_obj, something);
        }
    }

    FieldTypeT field_type() const override {
        if constexpr (std::is_same<std::string, T_field>::value) {
            return FieldTypeT::STRING;
        } else if constexpr (std::is_same<uint64_t, T_field>::value) {
            return FieldTypeT::UINT64;
        } else if constexpr (std::is_same<uint32_t, T_field>::value) {
            return FieldTypeT::UINT32;
        } else {
            return FieldTypeT::UNKNOWN;
        }
    }

    std::function<const T_field &(const container_class *const)> f_getter_;
    std::function<void(container_class *, const T_field &)> f_setter_;
    std::function<T_field *(container_class *)> f_getter_of_ptr_;

    ReflectFieldInfo(
        std::string name,
        std::function<const T_field &(const container_class *const)> f_getter,
        std::function<void(container_class *, const T_field &)> f_setter,
        std::function<T_field *(container_class *)> f_getter_of_ptr)
        : ReflectFieldInfoBase<container_class>(std::move(name)),
          f_getter_(std::move(f_getter)),
          f_setter_(std::move(f_setter)),
          f_getter_of_ptr_(std::move(f_getter_of_ptr)) {}
};

}  // namespace d_reflection

#define REFLECT_FIELD(m_ns, m_c, m_type, m_name)                                         \
private:                                                                                 \
    m_type m_name##_{};                                                                  \
                                                                                         \
public:                                                                                  \
    const m_type &m_name() const { return m_name##_; }                                   \
    void set##_##m_name(m_type const &value) { m_name##_ = value; }                      \
    m_type *m_name##_ptr() { return &m_name##_; }                                        \
                                                                                         \
private:                                                                                 \
    static inline d_reflection::ReflectFieldInfo<m_ns::m_c, m_type> m_name##_##field##_{ \
        #m_name, &m_ns::m_c::m_name, &m_ns::m_c::set_##m_name, &m_ns::m_c::m_name##_ptr};           \
                                                                                         \
public:                                                                                  \
    static d_reflection::ReflectFieldInfoBase<m_ns::m_c> *m_name##_##field() {           \
        return &m_name##_##field##_;                                                     \
    }

#define D_REFLECT_MAP_LINE(field) \
    { field()->name_, field() }
