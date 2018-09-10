#include <Servo.h>

#define DEFAULT_INPUT_NUMBER 2
#define TOTAL_INPUT_NUMBER DEFAULT_INPUT_NUMBER

class AbstractInputListener{
  public:
    AbstractInputListener(int paPinNumber, String paNom, int priorite);
    virtual ~AbstractInputListener();
    virtual void updateAngle() = 0;
    int getAngle() const;
    int getDefaultAngle() const;
    int getPriorite() const;
    String getNom() const;

 protected: 
    int _angle;
    int _defaultAngle; // angle par défaut, util pour prendre en compte le trim
    const int _pinNumber;
    const int _priorite;
    const String _nom; 
};

AbstractInputListener::AbstractInputListener(int paPinNumber, String paNom, int priorite) : _angle(0), _defaultAngle(90), _priorite(priorite), _pinNumber(paPinNumber), _nom(paNom)
{
 
};

AbstractInputListener::~AbstractInputListener()
{
 
};

int AbstractInputListener::getPriorite() const
{
  return _priorite;
};

int AbstractInputListener::getAngle() const
{
  return _angle;
};

int AbstractInputListener::getDefaultAngle() const
{
  return _defaultAngle;
};

String AbstractInputListener::getNom() const
{
  return _nom;
};

///////////////////////////////////////////////////////////////////////////

class RadioInputListener : public AbstractInputListener{
  public:
    RadioInputListener(int paPinNumber, String paNom);
    void updateAngle();
};

RadioInputListener::RadioInputListener(int paPinNumber, String paNom) : AbstractInputListener(paPinNumber, paNom, 4)
{
  int duree;
  pulseIn(_pinNumber, duree);
  int angle = map(duree, 1100, 1500, 0, 180);
  _defaultAngle = angle;
  Serial.print("angle par défaut "+_nom+" : "+angle+" \n");
};

void RadioInputListener::updateAngle()
{
  int duree;
  pulseIn(_pinNumber, duree);
  _angle = map(duree, 1100, 1500, 0, 180); 
  Serial.print("angle lu "+_nom+" : "+duree+" \n");
        Serial.write("radioElevator "+angleElevator);
      }else if (manualMode and inputList[i]->getNom() == "RadioRudder") // Ne prendre en compte que si on est en mode manuel
      {
        angleRudder = loAngle;
        prioriteRudder = loPriorite;
      }else if (manualMode and inputList[i]->getNom() == "RadioAileron") // Ne prendre en compte que si on est en mode manuel
      {
        // TODO
        // angleElevator = loAngle;
        // prioriteElevator = loPriorite;
      }else
      {
        //Serial.print("else case \n");
      }
//      else if (!manualMode and inputList[i]->getNom() == ...) // TODO, modèle pour les autres entrées
//      {
//        int &refAngle(...);
//        int &refPriorite(...);
//        UpdateAngle(refAngle, refPriorite, loAngle, loPriorite);   
//      }      
  }
  // Appliquer les nouveaux angles sur les sorties
  servoElevator->write(angleElevator);
  servoRudder->write(angleRudder);
};

///////////////////////////////////////////////////////////////////////////

// Entrées
int pinRadioElevatorListener(1);
int pinRadioRudderListener(2);

// Sorties
int pinServoElevator(6);
int pinServoRudder(7);
Servo servoElevator;
Servo servoRudder;

// Gestion d'évenements
AbstractInputListener *inputList[TOTAL_INPUT_NUMBER];

void setup() {
  // debug
  Serial.begin(9600);

  pinMode(pinRadioElevatorListener, INPUT);
  pinMode(pinRadioRudderListener, INPUT);
 
  inputList[0] = new RadioInputListener(pinRadioElevatorListener, "RadioElevator");
  inputList[1] = new RadioInputListener(pinRadioRudderListener, "RadioRudder");
 
  servoElevator.attach(pinServoElevator);
  servoRudder.attach(pinServoRudder);
};

void loop() {
  for (int i =0; i<TOTAL_INPUT_NUMBER; i++){
    inputList[i]->updateAngle();
  };

  //
  ManageInputs(inputList, &servoElevator, &servoRudder);

  delay(500);
};
