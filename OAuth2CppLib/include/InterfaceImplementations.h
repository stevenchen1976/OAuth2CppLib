#pragma once
#include "Types.h"
#include "Entities.h"
#include "Interfaces.h"
#include "OAuth2AuthServer.h"

#include <map>
#include <vector>
#include <regex>


namespace OAuth2
{
    using std::map;
    using std::vector;


// Require 'client_secret' parameter to be in request parameters
class RequestParameterClientAuthenticationFacade : public IClientAuthenticationFacade
{
public:
    virtual Client authenticateClient(const IHttpRequest &request) const;
    virtual ~RequestParameterClientAuthenticationFacade(){};
};


class DefaultClientAuthorizationFacade : public IClientAuthorizationFacade
{
private:
    /*std::set*/std::map<string,int> _grants;
    const string _authzPageBody;

    //All static constants should be documented in manual to prevent accidental changes
    static const string _acceptedFieldName; 
    static const string _userIdFieldName;
    
public:
    DefaultClientAuthorizationFacade(const string &authzPageBody);
    virtual bool isClientAuthorizedByUser(const Grant &grant) const;
    virtual void makeAuthorizationRequestPage(const Grant &grant, const IHttpRequest &request,IHttpResponse &response) const;
    virtual Errors::Code processAuthorizationRequest(const IHttpRequest& request, IHttpResponse &response) const;
    virtual ~DefaultClientAuthorizationFacade();
};


// Save generate and save code in memory storage
// RFC6749 4.1.3
class SimpleAuthorizationCodeGenerator : public IAuthorizationCodeGenerator
{
private:
    map<string,string> _codes;

public:
    SimpleAuthorizationCodeGenerator();
    virtual string generateAuthorizationCode(const Grant &grant, string &requestUri);
    virtual bool checkAndRemoveAuthorizationCode(const string &code, Grant &grant, string &requestUri);
    virtual void removeExpiredCodes();
    virtual ~SimpleAuthorizationCodeGenerator();
};

};
