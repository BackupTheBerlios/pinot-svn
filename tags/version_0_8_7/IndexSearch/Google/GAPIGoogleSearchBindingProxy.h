/* GAPIGoogleSearchBindingProxy.h
   Generated by gSOAP 2.7.9e from GoogleSearch.h
   Copyright(C) 2000-2006, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef GAPIGoogleSearchBinding_H
#define GAPIGoogleSearchBinding_H
#include "GAPIH.h"
extern SOAP_NMAC struct Namespace GAPI_namespaces[];
class GoogleSearchBinding
{   public:
	struct soap *soap;
	const char *endpoint;
	GoogleSearchBinding() { soap = soap_new(); if (soap) soap->namespaces = GAPI_namespaces; endpoint = "http://api.google.com/search/beta2"; };
	virtual ~GoogleSearchBinding() { if (soap) { soap_destroy(soap); soap_end(soap); soap_free(soap); } };
	virtual int gapi1__doGetCachedPage(std::string key, std::string url, xsd__base64Binary &return_) { return soap ? soap_call_gapi1__doGetCachedPage(soap, endpoint, NULL, key, url, return_) : SOAP_EOM; };
	virtual int gapi1__doSpellingSuggestion(std::string key, std::string phrase, std::string &return_) { return soap ? soap_call_gapi1__doSpellingSuggestion(soap, endpoint, NULL, key, phrase, return_) : SOAP_EOM; };
	virtual int gapi1__doGoogleSearch(std::string key, std::string q, int start, int maxResults, bool filter, std::string restrict_, bool safeSearch, std::string lr, std::string ie, std::string oe, struct gapi1__doGoogleSearchResponse &_param_1) { return soap ? soap_call_gapi1__doGoogleSearch(soap, endpoint, NULL, key, q, start, maxResults, filter, restrict_, safeSearch, lr, ie, oe, _param_1) : SOAP_EOM; };
};
#endif
