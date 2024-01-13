#include "alert.h"

void send_email(const char *from_email, const char *from_password, const char *to_email, const char *smtp_server, int smtp_port, const char *pdf_file_path)
{
    CURL *curl;
    CURLcode res;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        // Set email configuration
        struct curl_slist *recipients = NULL;
        recipients = curl_slist_append(recipients, to_email);
        char url[100];
        snprintf(url, 64, "smtps://%s:%d", smtp_server, smtp_port);

        // Set email headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: multipart/mixed");
        headers = curl_slist_append(headers, "From: ");
        headers = curl_slist_append(headers, "To: ");

        // Add Subject and Body headers
        char subject_header[100];
        snprintf(subject_header, 100, "Subject: Alert: Weather Abnormality Report");
        headers = curl_slist_append(headers, subject_header);
        headers = curl_slist_append(headers, ""); // Empty line to separate headers from body

        // Initialize the email body
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from_email);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // Set the headers
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // For Gmail, use the following options
        curl_easy_setopt(curl, CURLOPT_USERNAME, from_email);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, from_password);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

        // Attach the PDF file
        curl_mime *mime;
        curl_mimepart *part;

        mime = curl_mime_init(curl);

        // PDF attachment
        part = curl_mime_addpart(mime);
        curl_mime_name(part, "attachment");
        curl_mime_filedata(part, pdf_file_path);

        // Body text
        part = curl_mime_addpart(mime);
        curl_mime_data(part, "This is an automated alert. Attached below is the Weather Abnormality Report containing the daily forecast information as well as anomallies, indicating the data that caused them to be generated.", CURL_ZERO_TERMINATED);

        // Set the MIME data
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

        // Perform the email sending
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Cleanup
        curl_slist_free_all(recipients);
        curl_slist_free_all(headers); // Free the headers
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
}
