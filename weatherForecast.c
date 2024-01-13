#include <stdio.h>
#include <stdlib.h>
#include "getDataAPI.h"
#include "weatherProcessing.h"
#include "weatherReport.h"
#include "logger.h"
#include "alert.h"

int main(int argc, char *argv[])
{
    if (argc != 13)
    {
        fprintf(stderr, "Usage: %s <location> <date> <rawFilePath> <processedFilePath> <pdfFilePath> <fromEmail> <fromPassword> <toEmail> <smtpServer> <smtpPort> <weatherAPIBaseAddress> <weatherAPIKey>\n", argv[0]);
        return -1;
    }

    char *location = argv[1];
    char *date = argv[2];
    char *rawFilePath = argv[3];
    char *processedFilePath = argv[4];
    char *pdfFilePath = argv[5];

    char *fromEmail = argv[6];
    char *fromPassword = argv[7];
    char *toEmail = argv[8];
    char *smtpServer = argv[9];
    int smtpPort = atoi(argv[10]); // Convert string to integer
    char *weatherAPIBaseAddress = argv[11];
    char *weatherAPIKey = argv[12];

    // Check if 7 days have passed since the last call
    if (record_checker())
    {
        printf("All Files ran.\n");

        // If 7 days have passed, make the call to getDataAPI
        get_data(rawFilePath, location, date, weatherAPIBaseAddress, weatherAPIKey);
        retrieve_and_write_data(rawFilePath, processedFilePath);

        // Record the timestamp of the last call
        record_module_call();
    }
    else
    {
        printf("Less than seven days have been passed.\n");
    }

    // File name to save the PDF
    if (!create_report(processedFilePath, pdfFilePath))
        send_email(fromEmail, fromPassword, toEmail, smtpServer, smtpPort, pdfFilePath);
    else
    {
        fprintf(stderr, "Error in the create Report Function");
    }

    return 0;
}
