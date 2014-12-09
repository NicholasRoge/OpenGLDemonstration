#ifndef PROPERTY_H
#define PROPERTY_H

#include <functional>
#include <type_traits>


template<typename Type,typename std::enable_if<std::is_scalar<Type>::value>::type* = 0>
class Property
{
	public:
		typedef std::function<bool()> MutateHandler;

	private:
		ModificationRequestHandler handler;
		bool mutate_handler_active;
		Type value;

	public:
		Property(MutateHandler handler)
		{
			this->handler = handler;
			mutate_handler_active = false;
		}

		template<typename MutateHandlerClass>
		Property(MutateHandlerClass& instance,bool (MutateHandlerClass::* handler)())
		: Property(std::bind(handler,instance,std::placeholders::_1))
		{
		}

		template<typename MutateHandlerClass>
		Property(MutateHandlerClass* instance,bool (MutateHandlerClass::* handler)())
		: Property(std::bind(handler,instance,std::placeholders::_1))
		{
		}

		operator const Type&() const
		{
			return this->value;
		}

		Property<Type>& operator =(const Type& value)
		{
			this->value = value;

			if(this->mutate_handler_active)
			{
				this->mutate_handler_active = true;
				this->handler(value);
				this->mutate_handler_active = false;
			}

			return *this;
		}

		/*Property<Type>& operator +=(const Type& value)
		{
			return (*this = this->value + value);
		}

		Property<Type>& operator -=(const Type& value)
		{
			return (*this = this->value - value);
		}

		Property<Type>& operator *=(const Type& value)
		{
			return (*this = this->value * value);
		}

		Property<Type>& operator /=(const Type& value)
		{
			return (*this = this->value / value);
		}*/
};

#endif