#!/bin/bash

export LOCATION="Karachi"
export TIMESTAMP="next7days"
export RAW_FILE_PATH="RawDataFile.json"
export PROCESSED_FILE_PATH="ProcessedData.json"
export PDF_FILE_PATH="WeatherReport.pdf"
export FROM_EMAIL=""
export FROM_PASSWORD=""
export TO_EMAIL=""
export SMTP_SERVER="smtp.gmail.com"
export SMTP_PORT=465
export WEATHER_API_BASE_ADDRESS="https://weather.visualcrossing.com/VisualCrossingWebServices/rest/services/timeline/"
export WEATHER_API_KEY="GSF87S3N5CL3FLVMY4264S9RU"

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
