#ifndef NLIVE_QSERIALIZE_H_
#define NLIVE_QSERIALIZE_H_

#include <QString>
#include <string>
#include <boost/serialization/string.hpp>

namespace boost {
namespace serialization {

/**
 * QString
 */
template<class Archive>
inline void save( Archive& ar, const QString& s, const unsigned int /*version*/ )
{
	using boost::serialization::make_nvp;
	ar << make_nvp("value", s.toStdString());
}
 
template<class Archive>
inline void load( Archive& ar, QString& s, const unsigned int /*version*/ )
{
	using boost::serialization::make_nvp;
 
	std::string stdStr;
	ar >> make_nvp("value", stdStr);
	s = QString::fromStdString(stdStr);
}
 
template<class Archive>
inline void serialize( Archive& ar, QString& s, const unsigned int file_version )
{
	boost::serialization::split_free(ar, s, file_version);
}
 
} // namespace serialization
} // namespace boost

#endif