#pragma once

#include "ConfigurationFile.h"
#include "Core\Mapping.h"
#include "Core\Str.h"

//This class is used to read and write specific application Configurations. This class' name stands for "Configuration Instance".

namespace Mason::Configuration
{
	class ConfigIns
	{
	private:
		ConfigIns() = delete;
		ConfigIns(const ConfigIns& Obj) = delete;
		ConfigIns(const ConfigIns&& Obj) = delete;
		~ConfigIns() = delete;

		static ConfigurationFile* _File;
		static Core::String _BaseDirectory;
		static Core::String _IdmediateDirectory;
		static Core::String _ConfigPath;

		static Core::Mapping<Core::String, ConfigurationSegment*> _Properties;

		static void LoadProperties();

	public:
		static void Initalize();
		static void ShutDown();
		static void Save();

		inline static Core::String BaseDirectory() { return _BaseDirectory; }
		inline static Core::String IdmediateDirectory() { return _IdmediateDirectory; }

		static Core::Mapping<Core::String, Core::BasicObject*> DefaultSettings; //The String is the name of the property, and the BasicObject is the default value of the object.

		/// <summary>
		/// Pulls the targeted Property from the file. If the name does not exist, a new entry is recorded into the Configuration Instance.
		/// </summary>
		/// <param name="Name">The name of the target Property.</param>
		/// <returns>A reference to the BasicObject that is attached to the name. If the object is to be overriden, simply set the return value of this function equal to the new object. If a new record is constructed, the return value is new Core::AString. </returns>
		static Core::BasicObject* RetriveProperty(const Core::String& Name);

		static void ResetProperty(const Core::String& Name);
	};
}