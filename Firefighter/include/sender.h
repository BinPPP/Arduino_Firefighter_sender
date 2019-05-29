#define RF_RX 4
#define RF_TX 5
#define BT_RX 2
#define BT_TX 3

#define CH4_init_val 1.36f
#define IBUT_init_val 200.4665f
#define O2_init_val 3.0f
#define CO_init_val 4069.4069f

const char header[] = "Check Connection\n";
typedef struct package
{
  float CH4;
  float IBUT;
  float O2;
  float CO;
} GasValues;
;

union Gas {
  char gasBytes[4];
  float gasFloat;
};

void readInput();
void initSender(GasValues *gas_main);
void sendGasPackage(char *package);
void clearBuffer();
void sendPing(long interval);
void connectionCheck(long interval);
int pingCheck();
void listen();
void toggleListen(long interval);
int headerCheck();

enum DATASTATE
{
  AVAILABLE = 1,
  CLEARED = 0
};

typedef struct package GasValues;