#pragma once

#include <functional>

namespace CSharpStyleProperties {
	template<class classT, class ReturnT, class PassT, ReturnT(classT:: * getFn)(), void(classT:: * setFn)(PassT)>
	class Property {
		classT* instanceThisPtr;

	public:

		Property(classT* thisPtr) : instanceThisPtr(thisPtr) {}

		ReturnT get() const {
			return std::invoke(getFn, instanceThisPtr);
		}

		void set(PassT value) {
			std::invoke(setFn, instanceThisPtr, value);
		}

		Property& operator=(const Property& rhs) = delete;

		Property& operator=(PassT value) {
			set(value);
			return *this;
		}

		ReturnT operator*() const {
			return get();
		}

		operator ReturnT() const {
			return get();
		}
	};
}

#define CUSTOM_PROPERTY(VarName, ClassType, ReturnType, GetFnCode, PassType, SetFnCode) \
ReturnType PROPERTY_GET_##VarName##() GetFnCode \
void PROPERTY_SET_##VarName##(##PassType value) SetFnCode \
CSharpStyleProperties::Property<ClassType, ReturnType, PassType, &##ClassType##::PROPERTY_GET_##VarName##, &##ClassType##::PROPERTY_SET_##VarName##> VarName = \
CSharpStyleProperties::Property<ClassType, ReturnType, PassType, &##ClassType##::PROPERTY_GET_##VarName##, &##ClassType##::PROPERTY_SET_##VarName##>(this)

#define READ_ONLY_PROPERTY(VarName, ClassType, ReturnType, GetFnCode) \
ReturnType PROPERTY_GET_##VarName##() GetFnCode \
CSharpStyleProperties::Property<ClassType, ReturnType, void*, &##ClassType##::PROPERTY_GET_##VarName, nullptr> VarName = \
CSharpStyleProperties::Property<ClassType, ReturnType, void*, &##ClassType##::PROPERTY_GET_##VarName, nullptr>(this)

#define POLYMORPHIC_PROPERTY(VarName, ClassType, ReturnType, GetFnCode, PassType, SetFnCode) \
virtual ReturnType PROPERTY_GET_##VarName##() GetFnCode \
virtual void PROPERTY_SET_##VarName##(##PassType value) SetFnCode \
CSharpStyleProperties::Property<##ClassType##, ReturnType##, PassType##, &##ClassType##::PROPERTY_GET_##VarName##, &##ClassType##::PROPERTY_SET_##VarName##> VarName; \
typedef ReturnType PROPERTY_RETURN_##VarName##_TYPE; \
typedef PassType PROPERTY_PASS_##VarName##_TYPE

#define READ_ONLY_POLYMORPHIC_PROPERTY(VarName, ClassType, ReturnType, GetFnCode) \
virtual ReturnType PROPERTY_GET_##VarName##() GetFnCode \
CSharpStyleProperties::Property<ClassType, ReturnType, void*, &##ClassType##::PROPERTY_GET_##VarName, nullptr> VarName; \
typedef ReturnType PROPERTY_RETURN_##VarName##_TYPE

#define GET_OVERRIDE(VarName) \
virtual PROPERTY_RETURN_##VarName##_TYPE PROPERTY_GET_##VarName##() override

#define SET_OVERRIDE(VarName) \
virtual void PROPERTY_SET_##VarName##(PROPERTY_PASS_##VarName##_TYPE value) override