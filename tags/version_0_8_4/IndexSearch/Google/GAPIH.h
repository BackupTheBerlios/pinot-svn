/* GAPIH.h
   Generated by gSOAP 2.7.9e from GoogleSearch.h
   Copyright(C) 2000-2006, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef GAPIH_H
#define GAPIH_H
#include "GAPIStub.h"
#ifndef WITH_NOIDREF

#ifdef __cplusplus
extern "C" {
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_markelement(struct soap*, const void*, int);
SOAP_FMAC3 int SOAP_FMAC4 soap_putelement(struct soap*, const void*, const char*, int, int);
SOAP_FMAC3 void *SOAP_FMAC4 soap_getelement(struct soap*, int*);

#ifdef __cplusplus
}
#endif
SOAP_FMAC3 int SOAP_FMAC4 soap_putindependent(struct soap*);
SOAP_FMAC3 int SOAP_FMAC4 soap_getindependent(struct soap*);
#endif
SOAP_FMAC3 int SOAP_FMAC4 soap_ignore_element(struct soap*);

SOAP_FMAC3 void * SOAP_FMAC4 soap_instantiate(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 int SOAP_FMAC4 soap_fdelete(struct soap_clist*);
SOAP_FMAC3 void* SOAP_FMAC4 soap_class_id_enter(struct soap*, const char*, void*, int, size_t, const char*, const char*);

SOAP_FMAC3 void* SOAP_FMAC4 soap_container_id_forward(struct soap*, const char*, void*, size_t, int, int, size_t, unsigned int);

SOAP_FMAC3 void SOAP_FMAC4 soap_container_insert(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_byte
#define SOAP_TYPE_byte (2)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_byte(struct soap*, char *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_byte(struct soap*, const char *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_byte(struct soap*, const char*, int, const char *, const char*);
SOAP_FMAC3 char * SOAP_FMAC4 soap_get_byte(struct soap*, char *, const char*, const char*);
SOAP_FMAC3 char * SOAP_FMAC4 soap_in_byte(struct soap*, const char*, char *, const char*);

#ifndef SOAP_TYPE_int
#define SOAP_TYPE_int (1)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_int(struct soap*, int *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_int(struct soap*, const int *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_int(struct soap*, const char*, int, const int *, const char*);
SOAP_FMAC3 int * SOAP_FMAC4 soap_get_int(struct soap*, int *, const char*, const char*);
SOAP_FMAC3 int * SOAP_FMAC4 soap_in_int(struct soap*, const char*, int *, const char*);

#ifndef SOAP_TYPE_double
#define SOAP_TYPE_double (22)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_double(struct soap*, double *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_double(struct soap*, const double *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_double(struct soap*, const char*, int, const double *, const char*);
SOAP_FMAC3 double * SOAP_FMAC4 soap_get_double(struct soap*, double *, const char*, const char*);
SOAP_FMAC3 double * SOAP_FMAC4 soap_in_double(struct soap*, const char*, double *, const char*);

#ifndef SOAP_TYPE_unsignedByte
#define SOAP_TYPE_unsignedByte (9)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_unsignedByte(struct soap*, unsigned char *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_unsignedByte(struct soap*, const unsigned char *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_unsignedByte(struct soap*, const char*, int, const unsigned char *, const char*);
SOAP_FMAC3 unsigned char * SOAP_FMAC4 soap_get_unsignedByte(struct soap*, unsigned char *, const char*, const char*);
SOAP_FMAC3 unsigned char * SOAP_FMAC4 soap_in_unsignedByte(struct soap*, const char*, unsigned char *, const char*);

#ifndef SOAP_TYPE_unsignedInt
#define SOAP_TYPE_unsignedInt (8)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_unsignedInt(struct soap*, unsigned int *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_unsignedInt(struct soap*, const unsigned int *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_unsignedInt(struct soap*, const char*, int, const unsigned int *, const char*);
SOAP_FMAC3 unsigned int * SOAP_FMAC4 soap_get_unsignedInt(struct soap*, unsigned int *, const char*, const char*);
SOAP_FMAC3 unsigned int * SOAP_FMAC4 soap_in_unsignedInt(struct soap*, const char*, unsigned int *, const char*);

#ifndef SOAP_TYPE_bool
#define SOAP_TYPE_bool (18)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_bool(struct soap*, bool *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_bool(struct soap*, const bool *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_bool(struct soap*, const char*, int, const bool *, const char*);

SOAP_FMAC3S const char* SOAP_FMAC4S soap_bool2s(struct soap*, bool);
SOAP_FMAC3 bool * SOAP_FMAC4 soap_get_bool(struct soap*, bool *, const char*, const char*);
SOAP_FMAC3 bool * SOAP_FMAC4 soap_in_bool(struct soap*, const char*, bool *, const char*);

SOAP_FMAC3S int SOAP_FMAC4S soap_s2bool(struct soap*, const char*, bool *);

#ifndef SOAP_TYPE_std__string
#define SOAP_TYPE_std__string (19)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_std__string(struct soap*, std::string *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_std__string(struct soap*, const std::string *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_std__string(struct soap*, const std::string *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_std__string(struct soap*, const char*, int, const std::string*, const char*);
SOAP_FMAC3 std::string * SOAP_FMAC4 soap_get_std__string(struct soap*, std::string *, const char*, const char*);
SOAP_FMAC3 std::string * SOAP_FMAC4 soap_in_std__string(struct soap*, const char*, std::string*, const char*);
SOAP_FMAC5 std::string * SOAP_FMAC6 soap_new_std__string(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_std__string(struct soap*, std::string*);
SOAP_FMAC3 std::string * SOAP_FMAC4 soap_instantiate_std__string(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_std__string(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_gapi1__DirectoryCategory
#define SOAP_TYPE_gapi1__DirectoryCategory (17)
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_out_gapi1__DirectoryCategory(struct soap*, const char*, int, const gapi1__DirectoryCategory *, const char*);
SOAP_FMAC3 gapi1__DirectoryCategory * SOAP_FMAC4 soap_get_gapi1__DirectoryCategory(struct soap*, gapi1__DirectoryCategory *, const char*, const char*);
SOAP_FMAC3 gapi1__DirectoryCategory * SOAP_FMAC4 soap_in_gapi1__DirectoryCategory(struct soap*, const char*, gapi1__DirectoryCategory *, const char*);
SOAP_FMAC5 gapi1__DirectoryCategory * SOAP_FMAC6 soap_new_gapi1__DirectoryCategory(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_gapi1__DirectoryCategory(struct soap*, gapi1__DirectoryCategory*);
SOAP_FMAC3 gapi1__DirectoryCategory * SOAP_FMAC4 soap_instantiate_gapi1__DirectoryCategory(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_gapi1__DirectoryCategory(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_gapi1DirectoryCategoryArray
#define SOAP_TYPE_gapi1DirectoryCategoryArray (16)
#endif
SOAP_FMAC3 int SOAP_FMAC4 soap_out_gapi1DirectoryCategoryArray(struct soap*, const char*, int, const gapi1DirectoryCategoryArray *, const char*);
SOAP_FMAC3 gapi1DirectoryCategoryArray * SOAP_FMAC4 soap_get_gapi1DirectoryCategoryArray(struct soap*, gapi1DirectoryCategoryArray *, const char*, const char*);
SOAP_FMAC3 gapi1DirectoryCategoryArray * SOAP_FMAC4 soap_in_gapi1DirectoryCategoryArray(struct soap*, const char*, gapi1DirectoryCategoryArray *, const char*);
SOAP_FMAC5 gapi1DirectoryCategoryArray * SOAP_FMAC6 soap_new_gapi1DirectoryCategoryArray(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_gapi1DirectoryCategoryArray(struct soap*, gapi1DirectoryCategoryArray*);
SOAP_FMAC3 gapi1DirectoryCategoryArray * SOAP_FMAC4 soap_instantiate_gapi1DirectoryCategoryArray(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_gapi1DirectoryCategoryArray(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_gapi1ResultElementArray
#define SOAP_TYPE_gapi1ResultElementArray (15)
#endif
SOAP_FMAC3 int SOAP_FMAC4 soap_out_gapi1ResultElementArray(struct soap*, const char*, int, const gapi1ResultElementArray *, const char*);
SOAP_FMAC3 gapi1ResultElementArray * SOAP_FMAC4 soap_get_gapi1ResultElementArray(struct soap*, gapi1ResultElementArray *, const char*, const char*);
SOAP_FMAC3 gapi1ResultElementArray * SOAP_FMAC4 soap_in_gapi1ResultElementArray(struct soap*, const char*, gapi1ResultElementArray *, const char*);
SOAP_FMAC5 gapi1ResultElementArray * SOAP_FMAC6 soap_new_gapi1ResultElementArray(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_gapi1ResultElementArray(struct soap*, gapi1ResultElementArray*);
SOAP_FMAC3 gapi1ResultElementArray * SOAP_FMAC4 soap_instantiate_gapi1ResultElementArray(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_gapi1ResultElementArray(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_gapi1__ResultElement
#define SOAP_TYPE_gapi1__ResultElement (14)
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_out_gapi1__ResultElement(struct soap*, const char*, int, const gapi1__ResultElement *, const char*);
SOAP_FMAC3 gapi1__ResultElement * SOAP_FMAC4 soap_get_gapi1__ResultElement(struct soap*, gapi1__ResultElement *, const char*, const char*);
SOAP_FMAC3 gapi1__ResultElement * SOAP_FMAC4 soap_in_gapi1__ResultElement(struct soap*, const char*, gapi1__ResultElement *, const char*);
SOAP_FMAC5 gapi1__ResultElement * SOAP_FMAC6 soap_new_gapi1__ResultElement(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_gapi1__ResultElement(struct soap*, gapi1__ResultElement*);
SOAP_FMAC3 gapi1__ResultElement * SOAP_FMAC4 soap_instantiate_gapi1__ResultElement(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_gapi1__ResultElement(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_gapi1__GoogleSearchResult
#define SOAP_TYPE_gapi1__GoogleSearchResult (13)
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_out_gapi1__GoogleSearchResult(struct soap*, const char*, int, const gapi1__GoogleSearchResult *, const char*);
SOAP_FMAC3 gapi1__GoogleSearchResult * SOAP_FMAC4 soap_get_gapi1__GoogleSearchResult(struct soap*, gapi1__GoogleSearchResult *, const char*, const char*);
SOAP_FMAC3 gapi1__GoogleSearchResult * SOAP_FMAC4 soap_in_gapi1__GoogleSearchResult(struct soap*, const char*, gapi1__GoogleSearchResult *, const char*);
SOAP_FMAC5 gapi1__GoogleSearchResult * SOAP_FMAC6 soap_new_gapi1__GoogleSearchResult(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_gapi1__GoogleSearchResult(struct soap*, gapi1__GoogleSearchResult*);
SOAP_FMAC3 gapi1__GoogleSearchResult * SOAP_FMAC4 soap_instantiate_gapi1__GoogleSearchResult(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_gapi1__GoogleSearchResult(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_xsd__base64Binary
#define SOAP_TYPE_xsd__base64Binary (7)
#endif
SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__base64Binary(struct soap*, const char*, int, const xsd__base64Binary *, const char*);
SOAP_FMAC3 xsd__base64Binary * SOAP_FMAC4 soap_get_xsd__base64Binary(struct soap*, xsd__base64Binary *, const char*, const char*);
SOAP_FMAC3 xsd__base64Binary * SOAP_FMAC4 soap_in_xsd__base64Binary(struct soap*, const char*, xsd__base64Binary *, const char*);
SOAP_FMAC5 xsd__base64Binary * SOAP_FMAC6 soap_new_xsd__base64Binary(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_xsd__base64Binary(struct soap*, xsd__base64Binary*);
SOAP_FMAC3 xsd__base64Binary * SOAP_FMAC4 soap_instantiate_xsd__base64Binary(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_xsd__base64Binary(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (47)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Fault(struct soap*, struct SOAP_ENV__Fault *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Fault(struct soap*, const struct SOAP_ENV__Fault *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Fault(struct soap*, const struct SOAP_ENV__Fault *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Fault(struct soap*, const char*, int, const struct SOAP_ENV__Fault *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Fault * SOAP_FMAC4 soap_get_SOAP_ENV__Fault(struct soap*, struct SOAP_ENV__Fault *, const char*, const char*);
SOAP_FMAC3 struct SOAP_ENV__Fault * SOAP_FMAC4 soap_in_SOAP_ENV__Fault(struct soap*, const char*, struct SOAP_ENV__Fault *, const char*);
SOAP_FMAC5 struct SOAP_ENV__Fault * SOAP_FMAC6 soap_new_SOAP_ENV__Fault(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_SOAP_ENV__Fault(struct soap*, struct SOAP_ENV__Fault*);
SOAP_FMAC3 struct SOAP_ENV__Fault * SOAP_FMAC4 soap_instantiate_SOAP_ENV__Fault(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_SOAP_ENV__Fault(struct soap*, int, int, void*, size_t, const void*, size_t);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (46)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Reason(struct soap*, const struct SOAP_ENV__Reason *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Reason(struct soap*, const struct SOAP_ENV__Reason *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Reason(struct soap*, const char*, int, const struct SOAP_ENV__Reason *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Reason * SOAP_FMAC4 soap_get_SOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *, const char*, const char*);
SOAP_FMAC3 struct SOAP_ENV__Reason * SOAP_FMAC4 soap_in_SOAP_ENV__Reason(struct soap*, const char*, struct SOAP_ENV__Reason *, const char*);
SOAP_FMAC5 struct SOAP_ENV__Reason * SOAP_FMAC6 soap_new_SOAP_ENV__Reason(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_SOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason*);
SOAP_FMAC3 struct SOAP_ENV__Reason * SOAP_FMAC4 soap_instantiate_SOAP_ENV__Reason(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_SOAP_ENV__Reason(struct soap*, int, int, void*, size_t, const void*, size_t);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (45)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Detail(struct soap*, const struct SOAP_ENV__Detail *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Detail(struct soap*, const struct SOAP_ENV__Detail *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Detail(struct soap*, const char*, int, const struct SOAP_ENV__Detail *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Detail * SOAP_FMAC4 soap_get_SOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *, const char*, const char*);
SOAP_FMAC3 struct SOAP_ENV__Detail * SOAP_FMAC4 soap_in_SOAP_ENV__Detail(struct soap*, const char*, struct SOAP_ENV__Detail *, const char*);
SOAP_FMAC5 struct SOAP_ENV__Detail * SOAP_FMAC6 soap_new_SOAP_ENV__Detail(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_SOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail*);
SOAP_FMAC3 struct SOAP_ENV__Detail * SOAP_FMAC4 soap_instantiate_SOAP_ENV__Detail(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_SOAP_ENV__Detail(struct soap*, int, int, void*, size_t, const void*, size_t);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (43)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Code(struct soap*, const struct SOAP_ENV__Code *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Code(struct soap*, const struct SOAP_ENV__Code *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Code(struct soap*, const char*, int, const struct SOAP_ENV__Code *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Code * SOAP_FMAC4 soap_get_SOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *, const char*, const char*);
SOAP_FMAC3 struct SOAP_ENV__Code * SOAP_FMAC4 soap_in_SOAP_ENV__Code(struct soap*, const char*, struct SOAP_ENV__Code *, const char*);
SOAP_FMAC5 struct SOAP_ENV__Code * SOAP_FMAC6 soap_new_SOAP_ENV__Code(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_SOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code*);
SOAP_FMAC3 struct SOAP_ENV__Code * SOAP_FMAC4 soap_instantiate_SOAP_ENV__Code(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_SOAP_ENV__Code(struct soap*, int, int, void*, size_t, const void*, size_t);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (42)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Header(struct soap*, struct SOAP_ENV__Header *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Header(struct soap*, const struct SOAP_ENV__Header *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Header(struct soap*, const struct SOAP_ENV__Header *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Header(struct soap*, const char*, int, const struct SOAP_ENV__Header *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Header * SOAP_FMAC4 soap_get_SOAP_ENV__Header(struct soap*, struct SOAP_ENV__Header *, const char*, const char*);
SOAP_FMAC3 struct SOAP_ENV__Header * SOAP_FMAC4 soap_in_SOAP_ENV__Header(struct soap*, const char*, struct SOAP_ENV__Header *, const char*);
SOAP_FMAC5 struct SOAP_ENV__Header * SOAP_FMAC6 soap_new_SOAP_ENV__Header(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_SOAP_ENV__Header(struct soap*, struct SOAP_ENV__Header*);
SOAP_FMAC3 struct SOAP_ENV__Header * SOAP_FMAC4 soap_instantiate_SOAP_ENV__Header(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_SOAP_ENV__Header(struct soap*, int, int, void*, size_t, const void*, size_t);

#endif

#ifndef SOAP_TYPE_gapi1__doGoogleSearch
#define SOAP_TYPE_gapi1__doGoogleSearch (39)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_gapi1__doGoogleSearch(struct soap*, struct gapi1__doGoogleSearch *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_gapi1__doGoogleSearch(struct soap*, const struct gapi1__doGoogleSearch *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_gapi1__doGoogleSearch(struct soap*, const struct gapi1__doGoogleSearch *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_gapi1__doGoogleSearch(struct soap*, const char*, int, const struct gapi1__doGoogleSearch *, const char*);
SOAP_FMAC3 struct gapi1__doGoogleSearch * SOAP_FMAC4 soap_get_gapi1__doGoogleSearch(struct soap*, struct gapi1__doGoogleSearch *, const char*, const char*);
SOAP_FMAC3 struct gapi1__doGoogleSearch * SOAP_FMAC4 soap_in_gapi1__doGoogleSearch(struct soap*, const char*, struct gapi1__doGoogleSearch *, const char*);
SOAP_FMAC5 struct gapi1__doGoogleSearch * SOAP_FMAC6 soap_new_gapi1__doGoogleSearch(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_gapi1__doGoogleSearch(struct soap*, struct gapi1__doGoogleSearch*);
SOAP_FMAC3 struct gapi1__doGoogleSearch * SOAP_FMAC4 soap_instantiate_gapi1__doGoogleSearch(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_gapi1__doGoogleSearch(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_gapi1__doGoogleSearchResponse
#define SOAP_TYPE_gapi1__doGoogleSearchResponse (35)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_gapi1__doGoogleSearchResponse(struct soap*, struct gapi1__doGoogleSearchResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_gapi1__doGoogleSearchResponse(struct soap*, const struct gapi1__doGoogleSearchResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_gapi1__doGoogleSearchResponse(struct soap*, const struct gapi1__doGoogleSearchResponse *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_gapi1__doGoogleSearchResponse(struct soap*, const char*, int, const struct gapi1__doGoogleSearchResponse *, const char*);
SOAP_FMAC3 struct gapi1__doGoogleSearchResponse * SOAP_FMAC4 soap_get_gapi1__doGoogleSearchResponse(struct soap*, struct gapi1__doGoogleSearchResponse *, const char*, const char*);
SOAP_FMAC3 struct gapi1__doGoogleSearchResponse * SOAP_FMAC4 soap_in_gapi1__doGoogleSearchResponse(struct soap*, const char*, struct gapi1__doGoogleSearchResponse *, const char*);
SOAP_FMAC5 struct gapi1__doGoogleSearchResponse * SOAP_FMAC6 soap_new_gapi1__doGoogleSearchResponse(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_gapi1__doGoogleSearchResponse(struct soap*, struct gapi1__doGoogleSearchResponse*);
SOAP_FMAC3 struct gapi1__doGoogleSearchResponse * SOAP_FMAC4 soap_instantiate_gapi1__doGoogleSearchResponse(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_gapi1__doGoogleSearchResponse(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_gapi1__doSpellingSuggestion
#define SOAP_TYPE_gapi1__doSpellingSuggestion (34)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_gapi1__doSpellingSuggestion(struct soap*, struct gapi1__doSpellingSuggestion *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_gapi1__doSpellingSuggestion(struct soap*, const struct gapi1__doSpellingSuggestion *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_gapi1__doSpellingSuggestion(struct soap*, const struct gapi1__doSpellingSuggestion *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_gapi1__doSpellingSuggestion(struct soap*, const char*, int, const struct gapi1__doSpellingSuggestion *, const char*);
SOAP_FMAC3 struct gapi1__doSpellingSuggestion * SOAP_FMAC4 soap_get_gapi1__doSpellingSuggestion(struct soap*, struct gapi1__doSpellingSuggestion *, const char*, const char*);
SOAP_FMAC3 struct gapi1__doSpellingSuggestion * SOAP_FMAC4 soap_in_gapi1__doSpellingSuggestion(struct soap*, const char*, struct gapi1__doSpellingSuggestion *, const char*);
SOAP_FMAC5 struct gapi1__doSpellingSuggestion * SOAP_FMAC6 soap_new_gapi1__doSpellingSuggestion(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_gapi1__doSpellingSuggestion(struct soap*, struct gapi1__doSpellingSuggestion*);
SOAP_FMAC3 struct gapi1__doSpellingSuggestion * SOAP_FMAC4 soap_instantiate_gapi1__doSpellingSuggestion(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_gapi1__doSpellingSuggestion(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_gapi1__doSpellingSuggestionResponse
#define SOAP_TYPE_gapi1__doSpellingSuggestionResponse (33)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_gapi1__doSpellingSuggestionResponse(struct soap*, struct gapi1__doSpellingSuggestionResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_gapi1__doSpellingSuggestionResponse(struct soap*, const struct gapi1__doSpellingSuggestionResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_gapi1__doSpellingSuggestionResponse(struct soap*, const struct gapi1__doSpellingSuggestionResponse *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_gapi1__doSpellingSuggestionResponse(struct soap*, const char*, int, const struct gapi1__doSpellingSuggestionResponse *, const char*);
SOAP_FMAC3 struct gapi1__doSpellingSuggestionResponse * SOAP_FMAC4 soap_get_gapi1__doSpellingSuggestionResponse(struct soap*, struct gapi1__doSpellingSuggestionResponse *, const char*, const char*);
SOAP_FMAC3 struct gapi1__doSpellingSuggestionResponse * SOAP_FMAC4 soap_in_gapi1__doSpellingSuggestionResponse(struct soap*, const char*, struct gapi1__doSpellingSuggestionResponse *, const char*);
SOAP_FMAC5 struct gapi1__doSpellingSuggestionResponse * SOAP_FMAC6 soap_new_gapi1__doSpellingSuggestionResponse(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_gapi1__doSpellingSuggestionResponse(struct soap*, struct gapi1__doSpellingSuggestionResponse*);
SOAP_FMAC3 struct gapi1__doSpellingSuggestionResponse * SOAP_FMAC4 soap_instantiate_gapi1__doSpellingSuggestionResponse(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_gapi1__doSpellingSuggestionResponse(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_gapi1__doGetCachedPage
#define SOAP_TYPE_gapi1__doGetCachedPage (30)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_gapi1__doGetCachedPage(struct soap*, struct gapi1__doGetCachedPage *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_gapi1__doGetCachedPage(struct soap*, const struct gapi1__doGetCachedPage *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_gapi1__doGetCachedPage(struct soap*, const struct gapi1__doGetCachedPage *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_gapi1__doGetCachedPage(struct soap*, const char*, int, const struct gapi1__doGetCachedPage *, const char*);
SOAP_FMAC3 struct gapi1__doGetCachedPage * SOAP_FMAC4 soap_get_gapi1__doGetCachedPage(struct soap*, struct gapi1__doGetCachedPage *, const char*, const char*);
SOAP_FMAC3 struct gapi1__doGetCachedPage * SOAP_FMAC4 soap_in_gapi1__doGetCachedPage(struct soap*, const char*, struct gapi1__doGetCachedPage *, const char*);
SOAP_FMAC5 struct gapi1__doGetCachedPage * SOAP_FMAC6 soap_new_gapi1__doGetCachedPage(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_gapi1__doGetCachedPage(struct soap*, struct gapi1__doGetCachedPage*);
SOAP_FMAC3 struct gapi1__doGetCachedPage * SOAP_FMAC4 soap_instantiate_gapi1__doGetCachedPage(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_gapi1__doGetCachedPage(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_gapi1__doGetCachedPageResponse
#define SOAP_TYPE_gapi1__doGetCachedPageResponse (29)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_gapi1__doGetCachedPageResponse(struct soap*, struct gapi1__doGetCachedPageResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_gapi1__doGetCachedPageResponse(struct soap*, const struct gapi1__doGetCachedPageResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_gapi1__doGetCachedPageResponse(struct soap*, const struct gapi1__doGetCachedPageResponse *, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_gapi1__doGetCachedPageResponse(struct soap*, const char*, int, const struct gapi1__doGetCachedPageResponse *, const char*);
SOAP_FMAC3 struct gapi1__doGetCachedPageResponse * SOAP_FMAC4 soap_get_gapi1__doGetCachedPageResponse(struct soap*, struct gapi1__doGetCachedPageResponse *, const char*, const char*);
SOAP_FMAC3 struct gapi1__doGetCachedPageResponse * SOAP_FMAC4 soap_in_gapi1__doGetCachedPageResponse(struct soap*, const char*, struct gapi1__doGetCachedPageResponse *, const char*);
SOAP_FMAC5 struct gapi1__doGetCachedPageResponse * SOAP_FMAC6 soap_new_gapi1__doGetCachedPageResponse(struct soap*, int);
SOAP_FMAC5 void SOAP_FMAC6 soap_delete_gapi1__doGetCachedPageResponse(struct soap*, struct gapi1__doGetCachedPageResponse*);
SOAP_FMAC3 struct gapi1__doGetCachedPageResponse * SOAP_FMAC4 soap_instantiate_gapi1__doGetCachedPageResponse(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_gapi1__doGetCachedPageResponse(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_PointerToSOAP_ENV__Reason
#define SOAP_TYPE_PointerToSOAP_ENV__Reason (49)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Reason(struct soap*, const char *, int, struct SOAP_ENV__Reason *const*, const char *);
SOAP_FMAC3 struct SOAP_ENV__Reason ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason **, const char*, const char*);
SOAP_FMAC3 struct SOAP_ENV__Reason ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Reason(struct soap*, const char*, struct SOAP_ENV__Reason **, const char*);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_PointerToSOAP_ENV__Detail
#define SOAP_TYPE_PointerToSOAP_ENV__Detail (48)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Detail(struct soap*, const char *, int, struct SOAP_ENV__Detail *const*, const char *);
SOAP_FMAC3 struct SOAP_ENV__Detail ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail **, const char*, const char*);
SOAP_FMAC3 struct SOAP_ENV__Detail ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Detail(struct soap*, const char*, struct SOAP_ENV__Detail **, const char*);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_PointerToSOAP_ENV__Code
#define SOAP_TYPE_PointerToSOAP_ENV__Code (44)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Code(struct soap*, const char *, int, struct SOAP_ENV__Code *const*, const char *);
SOAP_FMAC3 struct SOAP_ENV__Code ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code **, const char*, const char*);
SOAP_FMAC3 struct SOAP_ENV__Code ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Code(struct soap*, const char*, struct SOAP_ENV__Code **, const char*);

#endif

#ifndef SOAP_TYPE_PointerTogapi1__GoogleSearchResult
#define SOAP_TYPE_PointerTogapi1__GoogleSearchResult (36)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTogapi1__GoogleSearchResult(struct soap*, gapi1__GoogleSearchResult *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTogapi1__GoogleSearchResult(struct soap*, gapi1__GoogleSearchResult *const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTogapi1__GoogleSearchResult(struct soap*, const char *, int, gapi1__GoogleSearchResult *const*, const char *);
SOAP_FMAC3 gapi1__GoogleSearchResult ** SOAP_FMAC4 soap_get_PointerTogapi1__GoogleSearchResult(struct soap*, gapi1__GoogleSearchResult **, const char*, const char*);
SOAP_FMAC3 gapi1__GoogleSearchResult ** SOAP_FMAC4 soap_in_PointerTogapi1__GoogleSearchResult(struct soap*, const char*, gapi1__GoogleSearchResult **, const char*);

#ifndef SOAP_TYPE_PointerToPointerTogapi1__DirectoryCategory
#define SOAP_TYPE_PointerToPointerTogapi1__DirectoryCategory (26)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToPointerTogapi1__DirectoryCategory(struct soap*, gapi1__DirectoryCategory **const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToPointerTogapi1__DirectoryCategory(struct soap*, gapi1__DirectoryCategory **const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToPointerTogapi1__DirectoryCategory(struct soap*, const char *, int, gapi1__DirectoryCategory **const*, const char *);
SOAP_FMAC3 gapi1__DirectoryCategory *** SOAP_FMAC4 soap_get_PointerToPointerTogapi1__DirectoryCategory(struct soap*, gapi1__DirectoryCategory ***, const char*, const char*);
SOAP_FMAC3 gapi1__DirectoryCategory *** SOAP_FMAC4 soap_in_PointerToPointerTogapi1__DirectoryCategory(struct soap*, const char*, gapi1__DirectoryCategory ***, const char*);

#ifndef SOAP_TYPE_PointerToPointerTogapi1__ResultElement
#define SOAP_TYPE_PointerToPointerTogapi1__ResultElement (25)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToPointerTogapi1__ResultElement(struct soap*, gapi1__ResultElement **const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToPointerTogapi1__ResultElement(struct soap*, gapi1__ResultElement **const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToPointerTogapi1__ResultElement(struct soap*, const char *, int, gapi1__ResultElement **const*, const char *);
SOAP_FMAC3 gapi1__ResultElement *** SOAP_FMAC4 soap_get_PointerToPointerTogapi1__ResultElement(struct soap*, gapi1__ResultElement ***, const char*, const char*);
SOAP_FMAC3 gapi1__ResultElement *** SOAP_FMAC4 soap_in_PointerToPointerTogapi1__ResultElement(struct soap*, const char*, gapi1__ResultElement ***, const char*);

#ifndef SOAP_TYPE_PointerTogapi1__ResultElement
#define SOAP_TYPE_PointerTogapi1__ResultElement (24)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTogapi1__ResultElement(struct soap*, gapi1__ResultElement *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTogapi1__ResultElement(struct soap*, gapi1__ResultElement *const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTogapi1__ResultElement(struct soap*, const char *, int, gapi1__ResultElement *const*, const char *);
SOAP_FMAC3 gapi1__ResultElement ** SOAP_FMAC4 soap_get_PointerTogapi1__ResultElement(struct soap*, gapi1__ResultElement **, const char*, const char*);
SOAP_FMAC3 gapi1__ResultElement ** SOAP_FMAC4 soap_in_PointerTogapi1__ResultElement(struct soap*, const char*, gapi1__ResultElement **, const char*);

#ifndef SOAP_TYPE_PointerTogapi1__DirectoryCategory
#define SOAP_TYPE_PointerTogapi1__DirectoryCategory (23)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTogapi1__DirectoryCategory(struct soap*, gapi1__DirectoryCategory *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTogapi1__DirectoryCategory(struct soap*, gapi1__DirectoryCategory *const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTogapi1__DirectoryCategory(struct soap*, const char *, int, gapi1__DirectoryCategory *const*, const char *);
SOAP_FMAC3 gapi1__DirectoryCategory ** SOAP_FMAC4 soap_get_PointerTogapi1__DirectoryCategory(struct soap*, gapi1__DirectoryCategory **, const char*, const char*);
SOAP_FMAC3 gapi1__DirectoryCategory ** SOAP_FMAC4 soap_in_PointerTogapi1__DirectoryCategory(struct soap*, const char*, gapi1__DirectoryCategory **, const char*);

#ifndef SOAP_TYPE_PointerTogapi1DirectoryCategoryArray
#define SOAP_TYPE_PointerTogapi1DirectoryCategoryArray (21)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTogapi1DirectoryCategoryArray(struct soap*, gapi1DirectoryCategoryArray *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTogapi1DirectoryCategoryArray(struct soap*, gapi1DirectoryCategoryArray *const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTogapi1DirectoryCategoryArray(struct soap*, const char *, int, gapi1DirectoryCategoryArray *const*, const char *);
SOAP_FMAC3 gapi1DirectoryCategoryArray ** SOAP_FMAC4 soap_get_PointerTogapi1DirectoryCategoryArray(struct soap*, gapi1DirectoryCategoryArray **, const char*, const char*);
SOAP_FMAC3 gapi1DirectoryCategoryArray ** SOAP_FMAC4 soap_in_PointerTogapi1DirectoryCategoryArray(struct soap*, const char*, gapi1DirectoryCategoryArray **, const char*);

#ifndef SOAP_TYPE_PointerTogapi1ResultElementArray
#define SOAP_TYPE_PointerTogapi1ResultElementArray (20)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTogapi1ResultElementArray(struct soap*, gapi1ResultElementArray *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTogapi1ResultElementArray(struct soap*, gapi1ResultElementArray *const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTogapi1ResultElementArray(struct soap*, const char *, int, gapi1ResultElementArray *const*, const char *);
SOAP_FMAC3 gapi1ResultElementArray ** SOAP_FMAC4 soap_get_PointerTogapi1ResultElementArray(struct soap*, gapi1ResultElementArray **, const char*, const char*);
SOAP_FMAC3 gapi1ResultElementArray ** SOAP_FMAC4 soap_in_PointerTogapi1ResultElementArray(struct soap*, const char*, gapi1ResultElementArray **, const char*);

#ifndef SOAP_TYPE_PointerTounsignedByte
#define SOAP_TYPE_PointerTounsignedByte (10)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTounsignedByte(struct soap*, unsigned char *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTounsignedByte(struct soap*, unsigned char *const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTounsignedByte(struct soap*, const char *, int, unsigned char *const*, const char *);
SOAP_FMAC3 unsigned char ** SOAP_FMAC4 soap_get_PointerTounsignedByte(struct soap*, unsigned char **, const char*, const char*);
SOAP_FMAC3 unsigned char ** SOAP_FMAC4 soap_in_PointerTounsignedByte(struct soap*, const char*, unsigned char **, const char*);

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__QName(struct soap*, char **);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__QName(struct soap*, char *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put__QName(struct soap*, char *const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__QName(struct soap*, const char*, int, char*const*, const char*);
SOAP_FMAC3 char ** SOAP_FMAC4 soap_get__QName(struct soap*, char **, const char*, const char*);
SOAP_FMAC3 char * * SOAP_FMAC4 soap_in__QName(struct soap*, const char*, char **, const char*);

#ifndef SOAP_TYPE_string
#define SOAP_TYPE_string (3)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_string(struct soap*, char **);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_string(struct soap*, char *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_string(struct soap*, char *const*, const char*, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_string(struct soap*, const char*, int, char*const*, const char*);
SOAP_FMAC3 char ** SOAP_FMAC4 soap_get_string(struct soap*, char **, const char*, const char*);
SOAP_FMAC3 char * * SOAP_FMAC4 soap_in_string(struct soap*, const char*, char **, const char*);

#endif

/* End of GAPIH.h */
