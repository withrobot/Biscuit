
#ifndef _CONFIGURATION_HPP_
#define _CONFIGURATION_HPP_

#include <set>
#include <map>
#include <string>

class Configuration
{
	public:
		Configuration();
		~Configuration();

		void Read(const std::string& file);

		const std::string& getHostname() const;
		bool getDebug() const;
		unsigned short getPort() const;
		const std::set<std::string>& getDevices() const;
		const std::string& getPassword() const;
		bool getMouseAcceleration() const;
		const std::string& getKeyboardLayout() const;

		const std::string getHotKeyName(unsigned int id) const;
		const std::string getHotKeyCommand(unsigned int id) const;
	private:
		std::string m_hostname;
		bool m_debug;
		unsigned short m_port;

		std::set<std::string> m_devices;
		std::string m_password;

		bool m_mouseAccelerate;
		std::string m_keyboardLayout;

		std::map<unsigned int, std::pair<std::string, std::string> > m_hotkeys;
};

#endif
