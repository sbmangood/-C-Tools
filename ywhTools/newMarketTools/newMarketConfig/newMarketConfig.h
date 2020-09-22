#pragma once

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>

struct NewMarketConfig_ 
{
	int useNewMarket;
	std::string ip;
	int port;
};
void to_json(nlohmann::json & j, const NewMarketConfig_ & p)
{
	j = nlohmann::json{ {"useNewMarket",p.useNewMarket},{"ip",p.ip},{"port",p.port} };
}

void from_json(const nlohmann::json & j, NewMarketConfig_ & p)
{
	j.at("useNewMarket").get_to(p.useNewMarket);
	j.at("ip").get_to(p.ip);
	j.at("port").get_to(p.port);
}
class NewMarketConfig
{
public:

	NewMarketConfig_ conf;


	bool getConfig()
	{
		try {
			std::ifstream  input(configFilePath);
			nlohmann::json j;
			input >> j;
			conf = j.get<NewMarketConfig_>();
		}
		catch (const std::exception & e) {
			std::cout << "cant read config file "<< configFilePath << e.what() << std::endl;
			conf.useNewMarket = 0;
			conf.ip = "127.0.0.1";
			conf.port = 10010;

		}
	}

private:
	const std::string configFilePath = "./Config/newMarketConfig.json";
};