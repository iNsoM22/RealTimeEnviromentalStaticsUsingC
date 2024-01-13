/**
 * @file alert.h
 *
 * @brief Functions for sending email alerts.
 */

#ifndef ALERT_H
#define ALERT_H

#include <stdio.h>
#include <curl/curl.h>

/**
 * @brief Send an email alert with an attachment.
 *
 * This function sends an email alert using the provided SMTP server and port.
 * The email contains a PDF attachment specified by the file path. It requires
 * authentication with the 'from_email' and 'from_password' credentials.
 *
 * @param from_email The sender's email address.
 * @param from_password The sender's email password.
 * @param to_email The recipient's email address.
 * @param smtp_server The SMTP server address.
 * @param smtp_port The SMTP server port.
 * @param pdf_file_path The file path of the PDF attachment.
 *
 * @note This function currently supports Gmail as the SMTP server.
 * Adjust the email headers, server settings, and authentication as needed.
 */
void send_email(const char *from_email, const char *from_password, const char *to_email, const char *smtp_server, int smtp_port, const char *pdf_file_path);

#endif
