extern ESP8266WebServer httpServer;

//void handlePage( String page);
bool processToken();
void handleLogin();
void handleNotFound();
void handleRoot() ;
void handleConf() ;
void handleGetConfFile() ;
void handleLogs() ;
void handleTasks() ;
void handleSaveConf();
void handleAddTask();
void handleDeleteTask();
void handleReboot() ;
void handleRebootASK() ;
void handleJSONListWifiNetworks();
void handleJSONRuntimeInfo();
void handleJSONListOtherDevices();
void handleConfUpload();

void handleFactoryReset();
//void handleRestAPI();

void handleConfUploadAfterFinished();
void handleJSONSwitchChange();

void handleJSONListDomoticzDevices();
void handleJSONListSheduledTasks();
void handleJSONListLogs();

bool is_authentified();
bool processAuth();
