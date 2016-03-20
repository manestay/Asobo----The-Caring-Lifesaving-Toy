void takeAction(int val) {
  String msg;
  String subj = "Asobo Alarm Warning";
  
  if (val >= 2) {
    attempted = false;
    msg = "WARNING: CHILD LEFT IN CAR AT FATAL LEVELS! Stop and return to your car now! The authorities have been notified.";
    alertAuth(val);
  }
  else { //warning, do not notify authorities right away
    msg = "WARNING: Child left in car and temperature levels rising. Stop and return to your car before fatal conditions!";
  }
  if (!attempted) {
    Serial.println(msg);
    TembooChoreo SendEmailChoreo;
    
    // Invoke the Temboo client
    SendEmailChoreo.begin();
  
    // Set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
    
    // Set Choreo inputs
    SendEmailChoreo.addInput("Username", GMAIL_USER_NAME);
    SendEmailChoreo.addInput("Password", GMAIL_PASSWORD);
    SendEmailChoreo.addInput("ToAddress", TO_EMAIL_ADDRESS);
    SendEmailChoreo.addInput("Subject", subj);
    SendEmailChoreo.addInput("MessageBody", msg);
    
    //SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
    unsigned int returnCode = SendEmailChoreo.run();
  
    // a return code of zero (0) means everything worked
    if (returnCode == 0) {
        Serial.println("Success! Email sent!");
    } else {
      //Serial.println(returnCode, DEC);
      /*while (SendEmailChoreo.available()) {
        char c = SendEmailChoreo.read();
        Serial.print(c);
      }*/
    } 
    SendEmailChoreo.close();
    attempted = true;
  }  
}

void alertAuth(int val) {
  const String TWILIO_ACCOUNT_SID = "AC4ee49341c096f16ad8d23de41354bc38";
  const String TWILIO_AUTH_TOKEN = "e8a0fe51418d76e03013afee60270127";
  const String TWILIO_NUMBER = "+1 469-208-6733";
  const String RECIPIENT_NUMBER = "+1 949-310-8713";  
  if (!attemptedSMS) {
    TembooChoreo SendSMSChoreo;
    SendSMSChoreo.begin();
  
    SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);  
    
    SendSMSChoreo.addInput("AccountSID", TWILIO_ACCOUNT_SID);
    SendSMSChoreo.addInput("AuthToken", TWILIO_AUTH_TOKEN);
    SendSMSChoreo.addInput("From", TWILIO_NUMBER);
    SendSMSChoreo.addInput("To", RECIPIENT_NUMBER);
    SendSMSChoreo.addInput("Body", "Child locked in car at Ecole Polytechnique. Send help immediately.");
    
    //SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");
    
    unsigned int returnCode = SendSMSChoreo.run();
    if (returnCode == 0) {
          Serial.println("Success! SMS sent!");
      } else {
        //Serial.println(returnCode, DEC);
      } 
    
    SendSMSChoreo.close();
    attemptedSMS = true;
  }
}

