#include "lib/data/corsika/steering_card.h"


#include <sstream>
#include <utility>
#include <fstream>

#include <iterator>

namespace lib
{
	namespace data
	{
		namespace corsika
		{

			steering_card::steering_card()
			{

			}

			void steering_card::insert(const std::string s)
			{
				std::istringstream ss(s);
				std::string tmp;


				while( std::getline(ss, tmp, '\n'))
				{
					m_oSteeringList.push_back(tmp);
					tmp.clear();
				}

			}

			const std::map<std::string, std::list<std::string> > steering_card::get_settings()
			{
				std::map<std::string, std::list<std::string> > returnMap;

				for(auto itr : m_oSteeringList)
				{
					std::istringstream ss(itr);

					std::list<std::string> split_option;

					std::copy( std::istream_iterator<std::string>(ss),
								std::istream_iterator<std::string>(),
								std::back_inserter(split_option));

					std::string option(split_option.front());
					split_option.pop_front();

					returnMap.insert( std::make_pair(option, split_option) );

				}

				return returnMap;
			}


			const steering_card::setting_iterator steering_card::begin()
			{
				return this->m_oSteeringList.begin();
			}


			const steering_card::setting_iterator steering_card::end()
			{
				return this->m_oSteeringList.end();
			}

			void steering_card::clear()
			{
				this->m_oSteeringList.clear();
			}
		}
	}
}

