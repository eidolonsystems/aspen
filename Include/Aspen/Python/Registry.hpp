#ifndef ASPEN_PYTHON_REGISTRY_HPP
#define ASPEN_PYTHON_REGISTRY_HPP
#include <optional>
#include <typeinfo>
#include <pybind11/pybind11.h>
#include "Aspen/Box.hpp"
#include "Aspen/Python/DllExports.hpp"
#include "Aspen/Python/ReactorPtr.hpp"

namespace Aspen {

  /**
   * Stores the function pointers needed to convert a Python object back to
   * a Boxed C++ reactor.
   */
  struct Boxers {

    /** Converts a Python object to a Box of its native type. */
    void (*m_boxer)(const pybind11::object&, void*, const std::type_info&);

    /** Converts a Python object to a Box<object>. */
    SharedBox<pybind11::object> (*m_object_boxer)(const pybind11::object&);

    /** Converts a Python object to a Box<void>. */
    SharedBox<void> (*m_void_boxer)(const pybind11::object&);
  };

  ASPEN_EXPORT_DLL void register_reactor(const pybind11::object& type,
    void (*boxer)(const pybind11::object&, void*, const std::type_info&),
    SharedBox<pybind11::object> (*objext_boxer)(const pybind11::object&),
    SharedBox<void> (*void_boxer)(const pybind11::object&));

  /**
   * Registers a reactor type so that it can be efficiently boxed.
   * @param type The type representing the reactor to register.
   * @param boxer The function object used to box reactors of the specified
   *        type.
   */
  template<typename T>
  void register_reactor(const pybind11::object& type) {
    register_reactor(type,
      [] (const pybind11::object& value, void* destination,
          const std::type_info& type) {
        if(type == typeid(SharedBox<reactor_result_t<T>>)) {
          auto pointer = value.cast<ReactorPtr<T>>();
          reinterpret_cast<std::optional<SharedBox<reactor_result_t<T>>>*>(
            destination)->emplace(*pointer);
        }
      },
      [] (const pybind11::object& value) {
        auto pointer = value.cast<ReactorPtr<T>>();
        return to_object(*pointer);
      },
      [] (const pybind11::object& value) {
        auto pointer = value.cast<ReactorPtr<T>>();
        return SharedBox<void>(*pointer);
      });
  }

  /** Returns the Boxers associated with a Python object. */
  ASPEN_EXPORT_DLL const Boxers& find_boxers(const pybind11::object& value);
}

#endif
