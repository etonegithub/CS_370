# Davis Garrett - 2025 

# I never do this many comments, but given it's group work I need SOMEONE other than myself to understand my god awful code. 

# Program is tested and currently functioning on the raspberry pi. Should work on local machine as well, put you might need to install python requests (py -m pip install requests) 

 

# Imports for program. Json for parsing response.json, OS used for file pathing, requests for HTML / API, shutil for moving files, time for delaying analysis results and retries. 

 

import json 

import os 

import requests 

import shutil 

import time 

 

# Setting up variables for program. 

# I do not care if you see the API key. This is a free service on a throwaway account on a throwaway email. I could make it pull from an external file, but given we're already sshing into the pi... well... 

 

apiKey = "6a87c9ca3012800309f9e714c21f2cc5f1507ae1d3b92b76c944d97452ea8af8" 

goodFold = "GOOD" 

badFold = "BAD" 

checkFold = "CHECK" 

 

# Check / Make folders. Realized that if someone else uses this code it won't be in the same environment as the pi. 

 

os.makedirs(goodFold, exist_ok = True) 

os.makedirs(badFold, exist_ok = True) 

os.makedirs(checkFold, exist_ok = True) 

 

# Look, I know this project demands creativity, but if I need to use an API I'm going to follow the instructions they suggest. 

# This block handles uploading the given file to the service, in this case Virustotal, to add the file to the systems analysis queue. 

# --- Limited to 32mb, 4 requests per minute, 400 per day ---,  

 

def fileUpload(filePath): 

 

    # Initial API url. 

 

    url = "https://www.virustotal.com/api/v3/files" 

 

    # Grab user filepath, mark the file, submit request with API key. 

 

    with open(filePath, "rb") as file: 

         

        files = {"file": (os.path.basename(filePath), file)} 

        headers = { 

            "accept": "application/json", 

            "x-apikey": apiKey 

 

        } 

 

        # Grab response from service. 

 

        response = requests.post(url, files = files, headers = headers) 

        return response.json() 

     

# Second API request, this block handles the analysis response, including the files relations, flags, contacts, etc... 

 

def fileAnalysis(id): 

 

    # This block functions nigh identical to the upload, we're just using the id we got from the initial reponse to check the file itself now. 

    # This block also might need review, as very rarely I'll get a response totalling zero (expected ~30-70 by file type). 

    # The above is a server error, apparently a known issue. Added section to retry. 

 

    url = f"https://www.virustotal.com/api/v3/analyses/{id}" 

 

    headers = { 

        "accept": "application/json", 

        "x-apikey": apiKey 

 

    } 

 

    response = requests.get(url, headers = headers) 

    return response.json() 

 

# Log the API response in the event any scanners are triggered into a txt file. Make file in same directory tested file moves to. 

 

def writeLog(logData, folder): 

 

    logPath = os.path.join(folder, "log.txt") 

    with open(logPath, "w") as logFile: 

        json.dump(logData, logFile) 

 

# Move the selected file based on the results of the scanner. 

# If the file trips any virus scanner, move it into the bad folder, if everything is clean move to the good folder. 

# I promise this will have more depth later, I just needed a simpler goal while working on getting the API to function... 

# Folder pathing is now predetermined. This is just the block that shuttles the file. 

 

def moveFile(filePath, folder): 

 

    shutil.move(filePath, os.path.join(folder, os.path.basename(filePath))) 

 

# I don't want to talk about this mess. Virustotal has a GOD AWFUL API, and I can't just take the original id request to grab this. 

# So this block recursively scans the CHECK folder after main, grabs the prior API call for the SHA-256, then one FINAL API for the behaviors. 

# This (on average) produces a 8000+ line log, of which we scan for impact severities high+, and then work off that. 

 

def processCheckFolder(): 

 

    print("CHECK Checked...") 

 

    for fileName in os.listdir(checkFold): 

        filePath = os.path.join(checkFold, fileName) 

        if os.path.isfile(filePath): 

 

            # Grab SHA-256 from file metadata in response 

            # Yes, we're still doing delays here because if we spam the API we get an hour ban. I'm not risking that during the demo. 

 

            upRes = fileUpload(filePath) 

            idCheck = upRes.get('data', {}).get('id', '') 

 

            time.sleep(15) 

 

            anaRes = fileAnalysis(idCheck) 

            sha256 = anaRes["meta"]["file_info"]["sha256"] 

 

            # The third API call, like all before. This one grabs the file behavior. 

 

            url = f"https://www.virustotal.com/api/v3/files/{sha256}/behaviour_summary" 

 

            headers = { 

                "accept": "application/json", 

                "x-apikey": apiKey 

                 

            } 

 

            response = requests.get(url, headers = headers) 

            logData = response.json() 

 

            # Create a new log for behavior response 

 

            logPath = os.path.join(checkFold, "checkLog.txt") 

            with open(logPath, "a") as logFile: 

                json.dump(logData, logFile) 

 

            # FINALLY, we read the folder for the impact severity. If we read nothing, it's goes to GOOD, otherwise it's deleted. 

            # Again, this isn't a fool proof solution, a legit file can cause false positives and malware can likewise go undetected. 

            # However, if it's still an issue past 70+ antivirus and sandbox testing, I give up. 

 

            resCheck = json.dumps(logData) 

 

            if "IMPACT_SEVERITY_HIGH" in resCheck or "IMPACT_SEVERITY_CRITICAL" in resCheck: 

                print("File breaches acceptable severity tolerance, deleting file.") 

                os.remove(filePath) 

 

            else: 

 

                print("File meets acceptable severity tolerance, passing file.") 

                moveFile(filePath, goodFold) 

 

# Behold, main() 

 

def main(filename): 

 

    # Variable setup for main. 

 

    retry = 0 

    malCon = 0 

    susCon = 0 

    undCon = 0 

 

    # Ask user for file path (this will likely change with the desktop implementation). 

 

    #filePath = input("Please enter the desired file path: ") 
    filePath = "TEMP/" + filename

 

    # Attempt to send the file to the server. 

 

    print("Sending file to server...") 

    resId = fileUpload(filePath) 

     

    # Pull the id number from the response. 

 

    id = resId['data']['id'] 

    print("File analysis queued. Waiting for evaluation...") 

 

    # Stall program for 30 seconds. Yes, just a blunt wait time. 

    # This is because Virustotal doesn't have any direct method of checking a files time in queue, so if the file is new to the system we need a buffer for analysis. 

    # It also helps us stop from spamming the service, as again we're limited to four request a minute. 

 

    time.sleep(30) 

 

    # Check results of analysis. In this case we're looking specifically for the category of, well, category. 

    # This field is the primary indicator for a file being malicious, suspicious, or clean. 

    # Also includes protection from getting a 0-0-0 reponse, will retry three times, logging then quitting if the limit is reached. 

 

    while retry < 3: 

 

        anaRes = fileAnalysis(id) 

 

        malCon = 0 

        susCon = 0 

        undCon = 0 

 

        # Total count based on results. 

 

        for engine, result in anaRes['data']['attributes']['results'].items(): 

            category = result.get("category") 

 

            if category == "malicious": 

                malCon += 1 

            elif category == "suspicious": 

                susCon += 1 

            elif category == "undetected": 

                undCon += 1 

 

        # In the event the server goofs and gives a 0-0-0 response, retry the request up to three times. 

        # Still waiting fifteen seconds between due to API limitations. 

 

        if (malCon + susCon + undCon) == 0: 

 

            retry += 1 

            print(f"Attempt {retry}/3: Server failed to retrieve file data, retrying in 15 seconds...") 

            time.sleep(15) 

 

        else: 

 

            break 

 

    # Failed flag. 

    # Will likely change to log into a different folder called ERROR if I write more error checks. 

 

    if retry == 3 and (malCon + susCon + undCon) == 0: 

 

        print("Unable to retrieve file data. Log written.") 

        print("Exiting program...") 

        writeLog(anaRes, checkFold) 

        return 

 

    # Decide how we want to classify and move the file, based on the type and amount of flags the file has. 

    # Generally it's difficult to know what is a "false positive", but given my own experience 4+ flags is a good sign to stay away, while 1-3 is worth checking. 

    # The elif statement will be expanded later to use further elements from the reponse, trying to better guess based on the files relations. (Currently just flagged as bad) 

    # Mal now deletes file and writes log, Sus now writes log and moves to new CHECK folder for future. 

 

    # Malicious flag. 

 

    if malCon > 3 or susCon > 6: 

 

        print("--- Results ---") 

        print(f"Malware: {malCon}, Suspicious: {susCon}, Undetected: {undCon}") 

        print(f"File is likely malicious, with {malCon + susCon} detections.") 

        writeLog(anaRes, badFold) 

        os.remove(filePath) 

 

    # False positive flag. 

 

    elif malCon > 0 or susCon > 2: 

 

        print("--- Results ---") 

        print(f"Malware: {malCon}, Suspicious: {susCon}, Undetected: {undCon}") 

        print(f"File may be malicious, possible false positive with {malCon + susCon} detection(s).") 

        writeLog(anaRes, checkFold) 

        moveFile(filePath, checkFold) 

 

    # Clean flag. 

 

    else: 

 

        print("--- Results ---") 

        print(f"Malware: {malCon}, Suspicious: {susCon}, Undetected: {undCon}") 

        print(f"File is likely clean, with no positive results.") 

        moveFile(filePath, goodFold) 

 

    # Verify files moved into CHECK. 

 

    print("Checking CHECK...") 

    processCheckFolder() 

 

if __name__ == "__main__": 

    main() 