#! /usr/local/bin/python
import sys
import os
import re

# # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# This ProcessError.py file as part of tiny-ci will     #
# Be run for every failed test                          #
#                                                       #
# It is up to the user to decide how the error should   #
# be handled in this script                             #
#                                                       #
# By default the script is set up to send an email      #
# with the error message (altough the SMTP settings     #
# first needs to be filled in)                          #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Here is the avalible information about the failure #
testName = sys.argv[1]
errorMessage = sys.argv[2]

# SMTP settings #
SMTPserver = ''
sender =     ''
destination = ['']

USERNAME = ""
PASSWORD = ""


# typical values for text_subtype are plain, html, xml
text_subtype = 'plain'

content="""\
A tiny-ci test has failed\n
""" + errorMessage

subject="Failed tiny-ci test '" + testName + "'";


## SEND THE EMAIL ##
from smtplib import SMTP_SSL as SMTP
from email.MIMEText import MIMEText

try:
    msg = MIMEText(content, text_subtype)
    msg['Subject'] = subject
    msg['From'] = sender

    conn = SMTP(SMTPserver)
    conn.set_debuglevel(False)
    conn.login(USERNAME, PASSWORD)
    try:
        conn.sendmail(sender, destination, msg.as_string())
    finally:
        conn.close()

except Exception, exc:
    sys.exit( "mail failed; %s" % str(exc) )
