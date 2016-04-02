//
// Created by olegartys on 27.03.16.
//

#ifndef QUERYANALYZINGPACKET_QUERYPARSER_H
#define QUERYANALYZINGPACKET_QUERYPARSER_H


#include "IParser.h"

class QueryParser : public IParser {
public:
    virtual int parse(const std::string& data, OnSuccess onSuccess, OnError onError) override;
    
};


#endif //QUERYANALYZINGPACKET_QUERYPARSER_H
