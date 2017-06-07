const int APINS[8] = {2,3,5,6,A3,4,8,7,};
const int CPINS[8] = {A0,A2,12,A1,11,13,10,9};
const int PIN_SIZE = 8;
const int PLANE_SIZE = 16;

typedef struct {
  String state;
  long int dem_vote;
  long int rep_vote;
} StateInfo;

int index;
bool wroteSerial = false;

StateInfo votes[50] = {
{.state = "AL", .dem_vote = 718084, .rep_vote = 1306925},
{.state = "AR", .dem_vote = 378729, .rep_vote = 677904},
{.state = "AZ", .dem_vote = 936250, .rep_vote = 1021154},
{.state = "CA", .dem_vote = 7230699, .rep_vote = 3841134},
{.state = "CO", .dem_vote = 1212209, .rep_vote = 1137455},
{.state = "CT", .dem_vote = 884432, .rep_vote = 668266},
{.state = "DC", .dem_vote = 260223, .rep_vote = 11553},
{.state = "DE", .dem_vote = 235581, .rep_vote = 185103},
{.state = "FL", .dem_vote = 4485745, .rep_vote = 4605515},
{.state = "GA", .dem_vote = 1837300, .rep_vote = 2068623},
{.state = "HI", .dem_vote = 266827, .rep_vote = 128815},
{.state = "IA", .dem_vote = 650790, .rep_vote = 798923},
{.state = "ID", .dem_vote = 189677, .rep_vote = 407199},
{.state = "IL", .dem_vote = 2977498, .rep_vote = 2118179},
{.state = "IN", .dem_vote = 1031953, .rep_vote = 1556220},
{.state = "KS", .dem_vote = 414788, .rep_vote = 656009},
{.state = "KY", .dem_vote = 628834, .rep_vote = 1202942},
{.state = "LA", .dem_vote = 779535, .rep_vote = 1178004},
{.state = "MA", .dem_vote = 1964768, .rep_vote = 1083069},
{.state = "MD", .dem_vote = 1497951, .rep_vote = 873646},
{.state = "ME", .dem_vote = 354873, .rep_vote = 334838},
{.state = "MI", .dem_vote = 2268193, .rep_vote = 2279805},
{.state = "MN", .dem_vote = 1366676, .rep_vote = 1322891},
{.state = "MO", .dem_vote = 1054889, .rep_vote = 1585753},
{.state = "MS", .dem_vote = 462001, .rep_vote = 678457},
{.state = "MT", .dem_vote = 174521, .rep_vote = 274120},
{.state = "NC", .dem_vote = 2162074, .rep_vote = 2339603},
{.state = "ND", .dem_vote = 93526, .rep_vote = 216133},
{.state = "NE", .dem_vote = 273858, .rep_vote = 485819},
{.state = "NH", .dem_vote = 348521, .rep_vote = 345789},
{.state = "NJ", .dem_vote = 2021756, .rep_vote = 1535513},
{.state = "NM", .dem_vote = 380724, .rep_vote = 315875},
{.state = "NV", .dem_vote = 537753, .rep_vote = 511319},
{.state = "NY", .dem_vote = 4143874, .rep_vote = 2640570},
{.state = "OH", .dem_vote = 2317001, .rep_vote = 2771984},
{.state = "OK", .dem_vote = 419788, .rep_vote = 947934},
{.state = "OR", .dem_vote = 934631, .rep_vote = 742506},
{.state = "PA", .dem_vote = 2844705, .rep_vote = 2912941},
{.state = "RI", .dem_vote = 249902, .rep_vote = 179421},
{.state = "SC", .dem_vote = 849469, .rep_vote = 1143611},
{.state = "SD", .dem_vote = 117442, .rep_vote = 227701},
{.state = "TN", .dem_vote = 867110, .rep_vote = 1517402},
{.state = "TX", .dem_vote = 3867816, .rep_vote = 4681590},
{.state = "UT", .dem_vote = 274188, .rep_vote = 452086},
{.state = "VA", .dem_vote = 1916845, .rep_vote = 1731156},
{.state = "VT", .dem_vote = 178179, .rep_vote = 95053},
{.state = "WA", .dem_vote = 1610524, .rep_vote = 1129120},
{.state = "WI", .dem_vote = 1382210, .rep_vote = 1409467},
{.state = "WV", .dem_vote = 187457, .rep_vote = 486198}
};

void configurePins() {
  for(int i = 0; i < PIN_SIZE; i++) {
    pinMode(CPINS[i],OUTPUT);
    pinMode(APINS[i],OUTPUT);
  }
}


void turnOff() {
  for(int i = 0; i < PIN_SIZE; i++) {
     digitalWrite(APINS[i],HIGH);
     digitalWrite(CPINS[i],HIGH);  
  }
}

void lightUp(int istart,int iend,int jstart, int jend,int count) {
  int loccnt = 0;
  for(int i = istart; i < iend; i++) {
     digitalWrite(APINS[i],LOW);
       for(int j = jstart; j < jend; j++) {
          if (loccnt++ == count) return;
          digitalWrite(CPINS[j],LOW);
          delay(.5);
          digitalWrite(CPINS[j],HIGH);
          
       }
     digitalWrite(APINS[i],HIGH);
   }
}

void nthRep(int n) {
  turnOff();
  /* Plane 3 */
  lightUp(PIN_SIZE/2,PIN_SIZE,0,PIN_SIZE/2,-1);
  /* Plane 2 */
  lightUp(PIN_SIZE/2,PIN_SIZE,PIN_SIZE/2,PIN_SIZE,-1);
  /* Plane 1 */
  int l = n % 32;
  if (l > 0) {
  /* Plane 1 */
    lightUp(0,PIN_SIZE/2,PIN_SIZE/2,PIN_SIZE,l);
  }
}

void nthDem(int n) {
  turnOff();
  /* Plane 0 */
  lightUp(0,PIN_SIZE/2,0,PIN_SIZE/2,-1);
  /* Plane 1 */
  lightUp(0,PIN_SIZE/2,PIN_SIZE/2,PIN_SIZE,-1);
  /* Plane 2 */
  int l = n % 32;
  if (l > 0) {
    lightUp(PIN_SIZE/2,PIN_SIZE,PIN_SIZE/2,PIN_SIZE,l);
  }
}

void displayVoterResults(int index) {
  StateInfo s = votes[index];
  long int dvotes = s.dem_vote;
  long int rvotes = s.rep_vote;
  long int total = dvotes + rvotes;
  if (dvotes > rvotes) {
    double r = (double) dvotes / total;
    int dpins = (int) round(r * 64);
    nthDem(dpins);
  } else {
    double r = (double) rvotes / total;
    int rpins = (int) round (r *64);
    nthRep(rpins);
  }
}

void setup() {
  configurePins();
  turnOff();
  Serial.begin(115200);
  Serial.setTimeout(100);
  index = 0;
}

void loop() {
  if (Serial.available()) { 
    String stateIndex = Serial.readString();
    String state = stateIndex.substring(0,2);
    index = stateIndex.substring(2).toInt();
    displayVoterResults(index); 
  } 
  if (index != 0) displayVoterResults(index); 
}
