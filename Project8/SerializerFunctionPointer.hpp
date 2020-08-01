#pragma once
#include <memory>
#include "ConnectMessage.hpp"

namespace mlcp
{
	typedef std::shared_ptr<GeneralMessage> GeneralMessagePtr;
	template<typename T>
	using templateMessagePtr = std::shared_ptr<T>;

	class SerializerFunctionPointerBase
	{
	public:
		virtual GeneralMessagePtr deserialize(const std::string& serializedMsg) = 0;
		virtual std::string serialize(GeneralMessagePtr msg) = 0;
	};

	template<typename T>
	class SerializerFunctionPointer : public SerializerFunctionPointerBase
	{
	public:
		std::function<templateMessagePtr<T>(const std::string&)> _ptr;

		explicit SerializerFunctionPointer(std::function<templateMessagePtr<T>(const std::string&)> ptr = nullptr)
			: SerializerFunctionPointerBase(), _ptr(ptr)
		{}

		GeneralMessagePtr deserialize(const std::string& serializedMsg) override
		{
			if(_ptr != nullptr)
				return _ptr(serializedMsg);
			return nullptr;
		}

		std::string serialize(GeneralMessagePtr msg) override
		{
			std::ostringstream os;
			boost::archive::binary_oarchive ia(os);
			auto temp = std::static_pointer_cast<T>(msg);
			ia << temp;
			return os.str() += temp->getType();
		}
	};
}

