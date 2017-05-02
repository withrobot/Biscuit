

#include "configuration.hpp"

#include <libconfig.h++>
#include <syslog.h>

Configuration::Configuration()
: m_hostname("localhost")
, m_debug(false)
, m_port(1978)
, m_mouseAccelerate(true)
, m_keyboardLayout("iso-8859-1")
{
	char hostname[256];
	gethostname(hostname, 256);
	m_hostname = hostname;
}

Configuration::~Configuration()
{
}

void Configuration::Read(const std::string& file)
{
	libconfig::Config config;
	config.readFile(file.c_str());

	if (config.exists("server.debug"))
	{
		m_debug = (bool)config.lookup("server.debug");
	}

	if (config.exists("server.port"))
	{
		m_port = (short)(unsigned int)config.lookup("server.port");
	}

	if (config.exists("device.id"))
	{
		libconfig::Setting& id = config.lookup("device.id");
		if (id.isScalar())
		{
			m_devices.insert((const char*)id);	
		}
		else if (id.isArray())
		{
			for (int i = 0; i < id.getLength(); i++)
			{
				m_devices.insert((const char*)id[i]);
			}
		}
		else
		{
			syslog(LOG_ERR, "device.id must be an array of strings or a single string");
		}
	}

	if (config.exists("device.password"))
	{
		m_password = (const char*)config.lookup("device.password");
	}

	if (config.exists("mouse.accelerate"))
	{
		m_mouseAccelerate = (bool)config.lookup("mouse.accelerate");
	}

	if (config.exists("keyboard.layout"))
	{
		m_keyboardLayout = (const char*)config.lookup("keyboard.layout");
	}

	if (config.exists("keyboard.hotkeys.key1.name") && config.exists("keyboard.hotkeys.key1.command"))
	{
		m_hotkeys[1] = std::make_pair(
				(const char*)config.lookup("keyboard.hotkeys.key1.name"),
				(const char*)config.lookup("keyboard.hotkeys.key1.command")
				);
	}

	if (config.exists("keyboard.hotkeys.key2.name") && config.exists("keyboard.hotkeys.key2.command"))
	{
		m_hotkeys[2] = std::make_pair(
				(const char*)config.lookup("keyboard.hotkeys.key2.name"),
				(const char*)config.lookup("keyboard.hotkeys.key2.command")
				);
	}

	if (config.exists("keyboard.hotkeys.key3.name") && config.exists("keyboard.hotkeys.key3.command"))
	{
		m_hotkeys[3] = std::make_pair(
				(const char*)config.lookup("keyboard.hotkeys.key3.name"),
				(const char*)config.lookup("keyboard.hotkeys.key3.command")
				);
	}

	if (config.exists("keyboard.hotkeys.key4.name") && config.exists("keyboard.hotkeys.key4.command"))
	{
		m_hotkeys[4] = std::make_pair(
				(const char*)config.lookup("keyboard.hotkeys.key4.name"),
				(const char*)config.lookup("keyboard.hotkeys.key4.command")
				);
	}

	if (config.exists("mouse.hotkeys.key1.command"))
	{
		m_hotkeys[5] = std::make_pair("",
				(const char*)config.lookup("mouse.hotkeys.key1.command")
				);
	}

	if (config.exists("mouse.hotkeys.key2.command"))
	{
		m_hotkeys[6] = std::make_pair("",
				(const char*)config.lookup("mouse.hotkeys.key2.command")
				);
	}
}

const std::string& Configuration::getHostname() const
{
	return m_hostname;
}

bool Configuration::getDebug() const
{
	return m_debug;
}

unsigned short Configuration::getPort() const
{
	return m_port;
}

const std::set<std::string>& Configuration::getDevices() const
{
	return m_devices;
}

const std::string& Configuration::getPassword() const
{
	return m_password;
}

bool Configuration::getMouseAcceleration() const
{
	return m_mouseAccelerate;
}

const std::string& Configuration::getKeyboardLayout() const
{
	return m_keyboardLayout;
}

const std::string Configuration::getHotKeyName(unsigned int id) const
{
	std::map<unsigned int, std::pair<std::string, std::string> >::const_iterator i;
	i = m_hotkeys.find(id);
	if (i == m_hotkeys.end())
		return "undefined";
	return i->second.first;
}

const std::string Configuration::getHotKeyCommand(unsigned int id) const
{
	std::map<unsigned int, std::pair<std::string, std::string> >::const_iterator i;
	i = m_hotkeys.find(id);
	if (i == m_hotkeys.end())
		return "";
	return i->second.second;
}
