//
// Created by olegartys on 02.04.16.
//

#ifndef QUERYANALYZINGPACKET_ISERIALIZABLE_H
#define QUERYANALYZINGPACKET_ISERIALIZABLE_H

#include <string>

/**
 * Interface represents serializable object.
 */
class ISerializable {
public:
    virtual ~ISerializable() = default;

    /**
     * Enumeration describes possible return values from deserialize method.
     */
    enum class ReturnCode { SUCCESS, DESERIALIZE_ERR };

protected:
    /**
     * @brief This method should serialize an object and return it's string representation.
     * @param styled Whether format and output string or not.
     */
    virtual std::string serialize(bool styled=false) const = 0;

    /**
     * @brief This method should fill an object from the given string.
     * @param src The given string representation of an object.
     * @return Code of result.
     */
    virtual ReturnCode deserialize(const std::string& src) = 0;

};

#endif //QUERYANALYZINGPACKET_ISERIALIZABLE_H
