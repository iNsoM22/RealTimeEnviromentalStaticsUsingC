#!/bin/bash

export LOCATION=""
export TIMESTAMP=""
export RAW_FILE_PATH=""
export PROCESSED_FILE_PATH=""
export PDF_FILE_PATH=""
export FROM_EMAIL=""
export FROM_PASSWORD=""
export TO_EMAIL=""
export SMTP_SERVER=""
export SMTP_PORT=465
export WEATHER_API_BASE_ADDRESS=""
export WEATHER_API_KEY=""

# Compile the weatherForecast program
gcc weatherForecast.c weatherProcessing.c weatherReport.c timeUNIXConversion.c storeResponse.c logger.c getDataAPI.c alert.c -lcurl -lcjson -lhpdf -o weatherForecast

# Check compilation status
if [ $? -eq 0 ]; then
    echo "Compilation successful."
else
    echo "Error: Compilation failed. Exiting."
    exit 1
fi

# If 7 days have passed, make the call to weatherForecast
./weatherForecast "$LOCATION" "$DATE" "$RAW_FILE_PATH" "$PROCESSED_FILE_PATH" "$PDF_FILE_PATH" "$FROM_EMAIL" "$FROM_PASSWORD" "$TO_EMAIL" "$SMTP_SERVER" "$SMTP_PORT" "$WEATHER_API_BASE_ADDRESS" "$WEATHER_API_KEY"
