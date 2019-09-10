// soapfault.hpp
//

///// Includes /////

#include "onvifserver/soapfault.hpp"

#include "onvifserver/onvifservice.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

SoapFault::SoapFault(const bool header, const FAULT fault, const std::string& detail) :
  header_(header),
  fault_(fault),
  detail_(detail)
{

}

SoapFault::SoapFault(const FAULT fault, const std::string& detail) :
  SoapFault(false, fault, detail)
{

}

SoapFault::~SoapFault()
{

}

std::string SoapFault::ToString() const
{
  return (std::string("<?xml version=\"1.0\"?>"
                      "<s:Envelope " + SOAP_NAMESPACE_ATTRIBUTE + " " + SCHEMA_NAMESPACE_ATTRIBUTE + " " + SCHEMA_INSTANCE_NAMESPACE_ATTRIBUTE + " " + BASEFAULT_NAMESPACE_ATTRIBUTE + " " + ERROR_NAMESPACE_ATTRIBUTE + " " + WSNT_NAMESPACE_ATTRIBUTE + " " + ADDRESSING_NAMESPACE_ATTRIBUTE + ">")
                       + (header_ ? "<s:Header><wsa:Action>http://www.w3.org/2005/08/addressing/soap/fault</wsa:Action></s:Header>" : "") +
                       "<s:Body>"
                         "<s:Fault>"
                           "<s:Code>"
                             "<s:Value>" + GetFaultCode() + "</s:Value>"
                             + GetSubCode() +
                           "</s:Code>"
                           "<s:Reason>"
                             "<s:Text xml:lang=\"en\">" + GetReason() + "</s:Text>"
                           "</s:Reason>"
                           "<s:Detail>"
                             + GetDetail() +
                           "</s:Detail>"
                         "</s:Fault>"
                       "</s:Body>"
                     "</s:Envelope>");
}

std::string SoapFault::GetFaultCode() const
{
  switch (fault_)
  {
    case FAULT_BADCONFIGURATION:
    case FAULT_CONFIGMODIFY:
    case FAULT_FIXEDSCOPE:
    case FAULT_FIXEDUSER:
    case FAULT_INVALIDARGS:
    case FAULT_INVALIDDATETIME:
    case FAULT_INVALIDGATEWAYADDRESS:
    case FAULT_INVALIDHOSTNAME:
    case FAULT_INVALIDIPV4ADDRESS:
    case FAULT_INVALIDNETWORKINTERFACE:
    case FAULT_INVALIDMESSAGECONTENTEXPRESSIONFAULT:
    case FAULT_INVALIDSOURCE:
    case FAULT_INVALIDSTREAMSETUP:
    case FAULT_INVALIDTOPICEXPRESSIONFAULT:
    case FAULT_INVALIDTIMEZONE:
    case FAULT_NAMESPACE:
    case FAULT_NOPROFILE:
    case FAULT_NORECORDING:
    case FAULT_NORECORDINGJOB:
    case FAULT_NOSCOPE:
    case FAULT_NOTAUTHORIZED:
    case FAULT_NOTRACK:
    case FAULT_NTPSERVERUNDEFINED:
    case FAULT_PASSWORDTOOLONG:
    case FAULT_PASSWORDTOOWEAK:
    case FAULT_PORTALREADYINUSE:
    case FAULT_SCOPEOVERWRITE:
    case FAULT_SERVICENOTSUPPORTED:
    case FAULT_UNKNOWNACTION:
    case FAULT_UNKNOWNTOKEN:
    case FAULT_USERNAMECLASH:
    case FAULT_USERNAMEMISSING:
    case FAULT_USERNAMETOOLONG:
    case FAULT_WELLFORMED:
    {

      return std::string("s:Sender");
    }
    case FAULT_ACTIONFAILED:
    case FAULT_ACTIONNOTSUPPORTED:
    case FAULT_CANNOTDELETERECEIVER:
    case FAULT_EMPTYSCOPE:
    case FAULT_MAXRECEIVERS:
    case FAULT_NOSUCHSERVICE:
    case FAULT_NOTIFYMESSAGENOTSUPPORTEDFAULT:
    case FAULT_TOOMANYUSERS:
    case FAULT_TOPICNOTSUPPORTEDFAULT:
    case FAULT_UNABLETODESTROYSUBSCRIPTIONFAULT:
    case FAULT_UNACCEPTABLEINITIALTERMINATIONTIME:
    case FAULT_UNACCEPTABLETERMINATIONTIME:
    {

      return std::string("s:Receiver");
    }
    default:
    {

      return std::string();
    }
  }
}

std::string SoapFault::GetSubCode() const
{
  switch (fault_)
  {
    case FAULT_ACTIONFAILED:
    {

      return std::string("<s:Subcode><s:Value>ter:Action</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_ACTIONNOTSUPPORTED:
    {

      return std::string("<s:Subcode><s:Value>ter:ActionNotSupported</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_BADCONFIGURATION:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_CANNOTDELETERECEIVER:
    {

      return std::string("<s:Subcode><s:Value>ter:Action</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_CONFIGMODIFY:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_EMPTYSCOPE:
    {

      return std::string("<s:Subcode><s:Value>ter:Action</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_FIXEDSCOPE:
    {

      return std::string("<s:Subcode><s:Value>ter:OperationProhibited</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_FIXEDUSER:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_INVALIDDATETIME:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_INVALIDGATEWAYADDRESS:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_INVALIDHOSTNAME:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_INVALIDARGS:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgs</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_INVALIDIPV4ADDRESS:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_INVALIDNETWORKINTERFACE:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_INVALIDMESSAGECONTENTEXPRESSIONFAULT:
    {

      return std::string();
    }
    case FAULT_INVALIDSOURCE:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_INVALIDSTREAMSETUP:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_INVALIDTIMEZONE:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_INVALIDTOPICEXPRESSIONFAULT:
    {

      return std::string();
    }
    case FAULT_MAXRECEIVERS:
    {

      return std::string("<s:Subcode><s:Value>ter:Action</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_NAMESPACE:
    {

      return std::string("<s:Subcode><s:Value>ter:Namespace</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_NOPROFILE:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_NORECORDING:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_NORECORDINGJOB:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_NOSCOPE:
    {
      
      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_NOSUCHSERVICE:
    {

      return std::string("<s:Subcode><s:Value>ter:ActionNotSupported</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_NOTAUTHORIZED:
    {

      return std::string("<s:Subcode><s:Value>ter:NotAuthorized</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_NOTIFYMESSAGENOTSUPPORTEDFAULT:
    {

      return std::string();
    }
    case FAULT_NOTRACK:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_NTPSERVERUNDEFINED:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_PASSWORDTOOLONG:
    {

      return std::string("<s:Subcode><s:Value>ter:OperationProhibited</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_PASSWORDTOOWEAK:
    {

      return std::string("<s:Subcode><s:Value>ter:OperationProhibited</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_PORTALREADYINUSE:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_SCOPEOVERWRITE:
    {

      return std::string("<s:Subcode><s:Value>ter:OperationProhibited</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_SERVICENOTSUPPORTED:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_TOOMANYUSERS:
    {

      return std::string("<s:Subcode><s:Value>ter:Action</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_TOPICNOTSUPPORTEDFAULT:
    {

      return std::string();
    }
    case FAULT_UNABLETODESTROYSUBSCRIPTIONFAULT:
    {

      return std::string();
    }
    case FAULT_UNACCEPTABLEINITIALTERMINATIONTIME:
    {

      return std::string();
    }
    case FAULT_UNACCEPTABLETERMINATIONTIME:
    {

      return std::string();
    }
    case FAULT_UNKNOWNACTION:
    {

      return std::string("<s:Subcode><s:Value>ter:UnknownAction</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_UNKNOWNTOKEN:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_USERNAMECLASH:
    {

      return std::string("<s:Subcode><s:Value>ter:OperationProhibited</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_USERNAMEMISSING:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidArgVal</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_USERNAMETOOLONG:
    {
      
      return std::string("<s:Subcode><s:Value>ter:OperationProhibited</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    case FAULT_WELLFORMED:
    {

      return std::string("<s:Subcode><s:Value>ter:WellFormed</s:Value>") + GetSubSubCode() + "</s:Subcode>";
    }
    default:
    {

      return std::string();
    }
  }
}

std::string SoapFault::GetSubSubCode() const
{
  switch (fault_)
  {
    case FAULT_ACTIONFAILED:
    {

      return std::string("<s:Subcode><s:Value>ter:ActionFailed</s:Value></s:Subcode>");
    }
    case FAULT_ACTIONNOTSUPPORTED:
    {

      return std::string();
    }
    case FAULT_BADCONFIGURATION:
    {

      return std::string("<s:Subcode><s:Value>ter:BadConfiguration</s:Value></s:Subcode>");
    }
    case FAULT_CANNOTDELETERECEIVER:
    {

      return std::string("<s:Subcode><s:Value>ter:CannotDeleteReceiver</s:Value></s:Subcode>");
    }
    case FAULT_CONFIGMODIFY:
    {
      
      return std::string("<s:Subcode><s:Value>ter:ConfigModify</s:Value></s:Subcode>");
    }
    case FAULT_EMPTYSCOPE:
    {

      return std::string("<s:Subcode><s:Value>ter:EmptyScope</s:Value></s:Subcode>");
    }
    case FAULT_FIXEDSCOPE:
    {

      return std::string("<s:Subcode><s:Value>ter:FixedScope</s:Value></s:Subcode>");
    }
    case FAULT_FIXEDUSER:
    {

      return std::string("<s:Subcode><s:Value>ter:FixedUser</s:Value></s:Subcode>");
    }
    case FAULT_INVALIDDATETIME:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidDateTime</s:Value></s:Subcode>");
    }
    case FAULT_INVALIDGATEWAYADDRESS:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidGatewayAddress</s:Value></s:Subcode>");
    }
    case FAULT_INVALIDHOSTNAME:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidHostname</s:Value></s:Subcode>");
    }
    case FAULT_INVALIDARGS:
    {

      return std::string();
    }
    case FAULT_INVALIDIPV4ADDRESS:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidIPv4Address</s:Value></s:Subcode>");
    }
    case FAULT_INVALIDNETWORKINTERFACE:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidNetworkInterface</s:Value></s:Subcode>");
    }
    case FAULT_INVALIDMESSAGECONTENTEXPRESSIONFAULT:
    {

      return std::string();
    }
    case FAULT_INVALIDSOURCE:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidSource</s:Value></s:Subcode>");
    }
    case FAULT_INVALIDSTREAMSETUP:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidStreamSetup</s:Value></s:Subcode>");
    }
    case FAULT_INVALIDTIMEZONE:
    {

      return std::string("<s:Subcode><s:Value>ter:InvalidTimeZone</s:Value></s:Subcode>");
    }
    case FAULT_INVALIDTOPICEXPRESSIONFAULT:
    {

      return std::string();
    }
    case FAULT_MAXRECEIVERS:
    {

      return std::string("<s:Subcode><s:Value>ter:MaxReceivers</s:Value></s:Subcode>");
    }
    case FAULT_NAMESPACE:
    {

      return std::string();
    }
    case FAULT_NOPROFILE:
    {

      return std::string("<s:Subcode><s:Value>ter:NoProfile</s:Value></s:Subcode>");
    }
    case FAULT_NORECORDING:
    {

      return std::string("<s:Subcode><s:Value>ter:NoRecording</s:Value></s:Subcode>");
    }
    case FAULT_NORECORDINGJOB:
    {

      return std::string("<s:Subcode><s:Value>ter:NoRecordingJob</s:Value></s:Subcode>");
    }
    case FAULT_NOSCOPE:
    {

      return std::string("<s:Subcode><s:Value>ter:NoScope</s:Value></s:Subcode>");
    }
    case FAULT_NOTAUTHORIZED:
    {

      return std::string();
    }
    case FAULT_NOTIFYMESSAGENOTSUPPORTEDFAULT:
    {

      return std::string();
    }
    case FAULT_NOTRACK:
    {

      return std::string();
    }
    case FAULT_NTPSERVERUNDEFINED:
    {

      return std::string("<s:Subcode><s:Value>ter:NtpServerUndefined</s:Value></s:Subcode>");
    }
    case FAULT_NOSUCHSERVICE:
    {

      return std::string("<s:Subcode><s:Value>ter:NoSuchService</s:Value></s:Subcode>");
    }
    case FAULT_PASSWORDTOOLONG:
    {

      return std::string("<s:Subcode><s:Value>ter:PasswordTooLong</s:Value></s:Subcode>");
    }
    case FAULT_PASSWORDTOOWEAK:
    {

      return std::string("<s:Subcode><s:Value>ter:PasswordTooWeak</s:Value></s:Subcode>");
    }
    case FAULT_PORTALREADYINUSE:
    {

      return std::string("<s:Subcode><s:Value>ter:PortAlreadyInUse</s:Value></s:Subcode>");
    }
    case FAULT_SCOPEOVERWRITE:
    {

      return std::string("<s:Subcode><s:Value>ter:ScopeOverwrite</s:Value></s:Subcode>");
    }
    case FAULT_SERVICENOTSUPPORTED:
    {

      return std::string("<s:Subcode><s:Value>ter:ServiceNotSupported</s:Value></s:Subcode>");
    }
    case FAULT_TOOMANYUSERS:
    {

      return std::string("<s:Subcode><s:Value>ter:TooManyUsers</s:Value></s:Subcode>");
    }
    case FAULT_TOPICNOTSUPPORTEDFAULT:
    {

      return std::string();
    }
    case FAULT_UNABLETODESTROYSUBSCRIPTIONFAULT:
    {

      return std::string();
    }
    case FAULT_UNACCEPTABLEINITIALTERMINATIONTIME:
    {

      return std::string();
    }
    case FAULT_UNACCEPTABLETERMINATIONTIME:
    {

      return std::string();
    }
    case FAULT_UNKNOWNACTION:
    {

      return std::string();
    }
    case FAULT_UNKNOWNTOKEN:
    {

      return std::string("<s:Subcode><s:Value>ter:UnknownToken</s:Value></s:Subcode>");
    }
    case FAULT_USERNAMECLASH:
    {

      return std::string("<s:Subcode><s:Value>ter:UsernameClash</s:Value></s:Subcode>");
    }
    case FAULT_USERNAMEMISSING:
    {

      return std::string("<s:Subcode><s:Value>ter:UsernameMissing</s:Value></s:Subcode>");
    }
    case FAULT_USERNAMETOOLONG:
    {

      return std::string("<s:Subcode><s:Value>ter:UsernameTooLong</s:Value></s:Subcode>");
    }
    case FAULT_WELLFORMED:
    {

      return std::string();
    }
    default:
    {

      return std::string();
    }
  }
}

std::string SoapFault::GetReason() const
{
  switch (fault_)
  {
    case FAULT_ACTIONFAILED:
    {

      return std::string("Action Failed");
    }
    case FAULT_ACTIONNOTSUPPORTED:
    {

      return std::string("Optional Action Not Implemented");
    }
    case FAULT_BADCONFIGURATION:
    {

      return std::string("The specified configuration is invalid.");
    }
    case FAULT_CANNOTDELETERECEIVER:
    {

      return std::string("It is not possible to delete the specified receiver, for example because it is currently in use.");
    }
    case FAULT_CONFIGMODIFY:
    {

      return std::string("The values in the configuration cannot be set");
    }
    case FAULT_EMPTYSCOPE:
    {

      return std::string("Scope list is empty.");
    }
    case FAULT_FIXEDSCOPE:
    {

      return std::string("Operation not Permitted");
    }
    case FAULT_FIXEDUSER:
    {

      return std::string("Username may not be deleted");
    }
    case FAULT_INVALIDDATETIME:
    {
      
      return std::string("An invalid date or time was specified.");
    }
    case FAULT_INVALIDGATEWAYADDRESS:
    {

      return std::string("The supplied gateway address was invalid.");
    }
    case FAULT_INVALIDHOSTNAME:
    {

      return std::string("The requested hostname cannot be accepted by the device.");
    }
    case FAULT_INVALIDARGS:
    {

      return std::string("Invalid Args");
    }
    case FAULT_INVALIDIPV4ADDRESS:
    {

      return std::string("The suggested IPv4 address is invalid");
    }
    case FAULT_INVALIDNETWORKINTERFACE:
    {

     return std::string("The supplied network interface token does not exist.");
    }
    case FAULT_INVALIDMESSAGECONTENTEXPRESSIONFAULT:
    {

      return std::string("Invalid message content filter");
    }
    case FAULT_INVALIDSOURCE:
    {

      return std::string("The recording source is invalid.");
    }
    case FAULT_INVALIDSTREAMSETUP:
    {

      return std::string("Specification of StreamType or Transport part in StreamSetup is not supported.");
    }
    case FAULT_INVALIDTIMEZONE:
    {

      return std::string("An invalid time zone was specified.");
    }
    case FAULT_INVALIDTOPICEXPRESSIONFAULT:
    {

      return std::string("Failed to parse topic expression");
    }
    case FAULT_MAXRECEIVERS:
    {

      return std::string("The maximum supported number of receivers has been reached.");
    }
    case FAULT_NAMESPACE:
    {

      return std::string("Namespace Error");
    }
    case FAULT_NOPROFILE:
    {

      return std::string("Profile token does not exist");
    }
    case FAULT_NORECORDING:
    {

      return std::string("The RecordingToken does not reference an existing recording");
    }
    case FAULT_NORECORDINGJOB:
    {

      return std::string("The JobToken does not reference an existing job");
    }
    case FAULT_NOSCOPE:
    {

      return std::string("Scope does not exist");
    }
    case FAULT_NOTAUTHORIZED:
    {

      return std::string("Sender not Authorized");
    }
    case FAULT_NOTIFYMESSAGENOTSUPPORTEDFAULT:
    {

      return std::string("Notify Message not supported");
    }
    case FAULT_NOTRACK:
    {

      return std::string("The TrackToken does not reference an existing track of the recording.");
    }
    case FAULT_NTPSERVERUNDEFINED:
    {

      return std::string("Cannot switch DateTimeType to NTP because no NTP server is defined");
    }
    case FAULT_NOSUCHSERVICE:
    {

      return std::string("The service is not supported");
    }
    case FAULT_PASSWORDTOOLONG:
    {

      return std::string("The password is too long");
    }
    case FAULT_PASSWORDTOOWEAK:
    {

      return std::string("Too weak password.");
    }
    case FAULT_PORTALREADYINUSE:
    {

      return std::string("The selected port is already in use.");
    }
    case FAULT_SCOPEOVERWRITE:
    {

      return std::string("Trying overwriting permanent device scope settings");
    }
    case FAULT_SERVICENOTSUPPORTED:
    {

      return std::string("The supplied network service is not supported.");
    }
    case FAULT_TOOMANYUSERS:
    {

      return std::string("Maximum number of supported users exceeded.");
    }
    case FAULT_TOPICNOTSUPPORTEDFAULT:
    {

      return std::string("Topic not supported");
    }
    case FAULT_UNABLETODESTROYSUBSCRIPTIONFAULT:
    {

      return std::string("Unable to destroy subscription");
    }
    case FAULT_UNACCEPTABLEINITIALTERMINATIONTIME:
    {

      return std::string("Invalid initial termination time");
    }
    case FAULT_UNACCEPTABLETERMINATIONTIME:
    {

      return std::string("Invalid termination time");
    }
    case FAULT_UNKNOWNTOKEN:
    {

      return std::string("The receiver indicated by ReceiverToken does not exist."); // This should only be used for receivers
    }
    case FAULT_USERNAMECLASH:
    {

      return std::string("Username already exists.");
    }
    case FAULT_USERNAMEMISSING:
    {

      return std::string("Username not recognized.");
    }
    case FAULT_USERNAMETOOLONG:
    {

      return std::string("The username is too long");
    }
    case FAULT_UNKNOWNACTION:
    {

      return std::string("Unknown Action");
    }
    case FAULT_WELLFORMED:
    {

      return std::string("Well-formed Error");
    }
    default:
    {

      return std::string();
    }
  }
}

std::string SoapFault::GetDetail() const
{
  switch (fault_)
  {
    case FAULT_ACTIONFAILED:
    case FAULT_ACTIONNOTSUPPORTED:
    case FAULT_BADCONFIGURATION:
    case FAULT_CANNOTDELETERECEIVER:
    case FAULT_CONFIGMODIFY:
    case FAULT_EMPTYSCOPE:
    case FAULT_FIXEDSCOPE:
    case FAULT_FIXEDUSER:
    case FAULT_INVALIDARGS:
    case FAULT_INVALIDDATETIME:
    case FAULT_INVALIDGATEWAYADDRESS:
    case FAULT_INVALIDHOSTNAME:
    case FAULT_INVALIDIPV4ADDRESS:
    case FAULT_INVALIDNETWORKINTERFACE:
    case FAULT_INVALIDTIMEZONE:
    case FAULT_MAXRECEIVERS:
    case FAULT_NAMESPACE:
    case FAULT_NOPROFILE:
    case FAULT_NORECORDING:
    case FAULT_NORECORDINGJOB:
    case FAULT_NOSCOPE:
    case FAULT_NOSUCHSERVICE:
    case FAULT_NOTAUTHORIZED:
    case FAULT_NOTRACK:
    case FAULT_NTPSERVERUNDEFINED:
    case FAULT_PASSWORDTOOLONG:
    case FAULT_PASSWORDTOOWEAK:
    case FAULT_PORTALREADYINUSE:
    case FAULT_SCOPEOVERWRITE:
    case FAULT_SERVICENOTSUPPORTED:
    case FAULT_TOOMANYUSERS:
    case FAULT_UNKNOWNACTION:
    case FAULT_UNKNOWNTOKEN:
    case FAULT_USERNAMECLASH:
    case FAULT_USERNAMEMISSING:
    case FAULT_USERNAMETOOLONG:
    case FAULT_WELLFORMED:
      return ("<s:Text>" + detail_ + "</s:Text>");
    case FAULT_INVALIDTOPICEXPRESSIONFAULT:
    case FAULT_NOTIFYMESSAGENOTSUPPORTEDFAULT:
    case FAULT_TOPICNOTSUPPORTEDFAULT:
    case FAULT_UNABLETODESTROYSUBSCRIPTIONFAULT:
    case FAULT_UNACCEPTABLEINITIALTERMINATIONTIME:
    case FAULT_UNACCEPTABLETERMINATIONTIME:
      return detail_;
    default:
    {

      return std::string();
    }
  }
}

}

}
