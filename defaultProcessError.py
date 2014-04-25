#! /usr/local/bin/python

#SMTPClient mailc("my.inbox.com",465,"tiny-ci-test@inbox.com","tinytestpassword");
#    cout << mailc.Send("tiny-ci-test@inbox.com","tiny-ci-test@inbox.com","test","Hello from C++ SMTP Client!") << endl;

SMTPserver = ''
sender =     ''
destination = ['']

USERNAME = ""
PASSWORD = ""

# typical values for text_subtype are plain, html, xml
text_subtype = 'plain'


content="""\
Test message
"""

subject="Sent from Python"

import sys
import os
import re

from smtplib import SMTP_SSL as SMTP       # this invokes the secure SMTP protocol (port 465, uses SSL)
# from smtplib import SMTP                  # use this for standard SMTP protocol   (port 25, no encryption)
from email.MIMEText import MIMEText

try:
    msg = MIMEText(content, text_subtype)
    msg['Subject']=       subject
    msg['From']   = sender # some SMTP servers will do this automatically, not all

    conn = SMTP(SMTPserver)
    conn.set_debuglevel(False)
    conn.login(USERNAME, PASSWORD)
    try:
        conn.sendmail(sender, destination, msg.as_string())
    finally:
        conn.close()

except Exception, exc:
    sys.exit( "mail failed; %s" % str(exc) ) # give a error message
