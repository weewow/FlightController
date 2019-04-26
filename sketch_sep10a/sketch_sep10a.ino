#include <Servo.h>

#define DEFAULT_INPUT_NUMBER 2
#define RADIO_INPUT_MIN_PWM 1000 // durée min d'un signal de radiocommande (empirique)
#define RADIO_INPUT_MAX_PWM 2000 // durée max d'un signal de radiocommande (empirique)
#define DEBUG_MODE

// Permet de Rajouter autant d'entrées dans le tableau que nécessaire
// A compléter avec le corps de la fonction setup()
const int totalInputNumber = DEFAULT_INPUT_NUMBER
#ifdef TOTO
  + TOTO
#endif
  ;


class AbstractInputListener{
  public:
    AbstractInputListener(int paPinNumber, String paNom, int priorite);
    virtual void updateAngle() = 0;
    int getAngle() const;
    int getDefaultAngle() const;
    int getPriorite() const;
    String getNom() const;

 protected:  
    int _angle; // angle enregistré au dernier appel de updateAngle 
    int _defaultAngle; // angle par défaut, util pour prendre en compte le trim
    const int _pinNumber;
    const int _priorite;
    const String _nom;  
};

AbstractInputListener::AbstractInputListener(int paPinNumber, String paNom, int priorite) : 
  _angle(0), 
  _defaultAngle(90),  
  _pinNumber(paPinNumber),
  _priorite(priorite), 
  _nom(paNom)
{
  pinMode(_pinNumber, INPUT); 
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
    virtual void updateAngle();
};

void PrintDebug(String msg)
{
  #ifdef DEBUG_MODE
    Serial.println(msg);
  #endif;
};

RadioInputListener::RadioInputListener(int paPinNumber, String paNom) : 
  AbstractInputListener(paPinNumber, paNom, 4)
{
  int duree(1300); // Durée d'une impulsion
  
    duree = pulseIn(_pinNumber, HIGH); // Lecture sur pin digital seulement
  if ((duree >= RADIO_INPUT_MIN_PWM) and (duree <= RADIO_INPUT_MAX_PWM)){ // Filtrer les valeurs parasites
    int angle = map(duree, RADIO_INPUT_MIN_PWM, RADIO_INPUT_MAX_PWM, 0, 180); // Conversion de la durée en angle, les durées min et max sont empiriques
    _defaultAngle = angle; // La trim sur les différents volets de control doit etre déjà réglée pour etre considérée comme l'angle de référence
  }
};

void RadioInputListener::updateAngle()
{
  int duree(1300);
  
    duree = pulseIn(_pinNumber, HIGH); // Lecture sur pin digital seulement
  if ((duree >= RADIO_INPUT_MIN_PWM) and (duree <= RADIO_INPUT_MAX_PWM)){ // Filtrer les valeurs parasites
    _angle = map(duree, RADIO_INPUT_MIN_PWM, RADIO_INPUT_MAX_PWM, 0, 180);  
    //if(getNom() == "RadioElevator")
      //PrintDebug(getNom()+" angle mis-à-jour : "+_angle+" ° | durée mesurée : "+duree);
  }
};

///////////////////////////////////////////////////////////////////////////
// Détermine si l'utilisateur envoi une commande ou non
// Parcours la liste des éléments
// Si une action est faite par l'utilisateur alors renvoyer true (on est en mode manuel)
// Il faut scruter les éléments RadioElevator, RadioRudder et RadioAileron et voir si l'angle est différent de l'angle par défaut
bool ManualMode(AbstractInputListener *inputList[])
{
  bool result(false);
  int deltaAngle(5);
  //String debugText("Angles manuels trouvés : ");
  //int debugNbEntrees(0); 
  for (int i=0; i< totalInputNumber; i++)
  {
    // or (inputList[i]->getNom() == "RadioRudder") or (inputList[i]->getNom() == "RadioAileron")
    if ((inputList[i]->getNom() == "RadioElevator") )
    {
      // On considère une marge d'incertitude sur l'angle : deltaAngle
      //debugNbEntrees++;
      //debugText += " | " + inputList[i]->getAngle();
      result = result || 
               (inputList[i]->getAngle() > inputList[i]->getDefaultAngle() + deltaAngle) ||
               (inputList[i]->getAngle() < inputList[i]->getDefaultAngle() - deltaAngle); 
      //
      //PrintDebug("");
      //PrintDebug("nouvel angle :     "+String(inputList[i]->getAngle()));
      //PrintDebug("angle par défaut : "+String(inputList[i]->getDefaultAngle())); 
//      PrintDebug("--");
//      PrintDebug(String(result));
//      PrintDebug(String(inputList[i]->getAngle() > inputList[i]->getDefaultAngle() + deltaAngle));
//      PrintDebug(String(inputList[i]->getAngle() < inputList[i]->getDefaultAngle() - deltaAngle));
      //
      if (result)
      {
        break; 
      }
    }
  }
  //String toto(debugNbEntrees);
  //PrintDebug(5);
  return result;
}

// Fonction de modification d'un angle en fonction d'un autre angle et d'une priorité
// Si on propose un angle avec une priorité supérieure alors on écrase l'angle et la priorité actuelle
// Si on propose un angle avec une priorité égale alors on moyenne la valeur des angles
// Si on propose un angle avec une priorité inférieur alors on ne fait rien
void UpdateAngle(int &paAngleOrigine, int &paPrioriteOrigine, int paNouvelAngle, int paNouvellePriorite)
{
  if (paNouvellePriorite == paPrioriteOrigine) // Moyenner les valeurs
  {
    paAngleOrigine = (paNouvelAngle + paAngleOrigine)/2; 
  }else if (paNouvellePriorite > paPrioriteOrigine) // Ecraser les anciennes Valeurs
  {
    paPrioriteOrigine = paNouvellePriorite;
    paAngleOrigine = paNouvelAngle;  
  } 
}

// Pour chacuns des éléments de inputList,
// on vas tenter de mettre à jour (si besoin) l'angle de sortie ainsi que la priorité
// un élément peut avoir un impact sur une ou plusieurs sorties
// si on est en mode manuel, ne prendre en compte que les entrées radio
// Si on est pas en mode manuel, prendre en compte toutes les entrées sauf les entrées radio
void ManageInputs(AbstractInputListener *inputList[], Servo *servoElevator, Servo *servoRudder)
{
  int angleElevator(-1);
  //int prioriteElevator(-1);
  //
  int angleRudder(-1);
  //int prioriteRudder(-1);
  //
  bool manualMode(ManualMode(inputList));
  PrintDebug("mode manuel ? "+String(manualMode));
  //

  for (int i=0; i< totalInputNumber; i++) // Modification des sorties en fonction des entrées
  {
      int loAngle(inputList[i]->getAngle());
//      int loPriorite(inputList[i]->getPriorite());
      //
      if (manualMode and inputList[i]->getNom() == "RadioElevator") // Ne prendre en compte que si on est en mode manuel
      {        
        angleElevator = loAngle;
//        prioriteElevator = loPriorite;
      }else if (manualMode and inputList[i]->getNom() == "RadioRudder") // Ne prendre en compte que si on est en mode manuel
      {        
        angleRudder = loAngle;
//        prioriteRudder = loPriorite;
      }else if (manualMode and inputList[i]->getNom() == "RadioAileron") // Ne prendre en compte que si on est en mode manuel
      {
        // TODO
        // angleElevator = loAngle;
        // prioriteElevator = loPriorite;
      }else{
        
      }
      // TODO, gérer les entrées automatiques
//      else if (!manualMode and inputList[i]->getNom() == ...) modèle pour les autres entrées
//      {
//        UpdateAngle(angleElevator, prioriteElevator, loAngle, loPriorite);  
//        ou  
//        UpdateAngle(angleRudder, prioriteRudder, loAngle, loPriorite);  
//        ou  
//        ... 
//      }       
  }
  // Appliquer les nouveaux angles sur les sorties
  servoElevator->write(angleElevator);
  servoRudder->write(angleRudder);
};

///////////////////////////////////////////////////////////////////////////

// Entrées (numériques)
int pinRadioElevatorListener(3); // D3
int pinRadioRudderListener(4); // D4

// Sorties (numériques)
int pinServoElevator(6); // D6
int pinServoRudder(7);   // D7
Servo servoElevator;
Servo servoRudder;

// Gestion d'évenements
AbstractInputListener *inputList[totalInputNumber];

void setup() {
  pinMode(pinRadioElevatorListener, INPUT);
  pinMode(pinRadioRudderListener, INPUT);
  inputList[0] = new RadioInputListener(pinRadioElevatorListener, "RadioElevator");
  inputList[1] = new RadioInputListener(pinRadioRudderListener, "RadioRudder");
#ifdef TOTO
  // Permet de rajouter autant d'entrées que nécessaire
  // inputList[1] = new ...
#endif
#ifdef DEBUG_MODE
  Serial.begin(9600);
#endif;
  servoElevator.attach(pinServoElevator);
  servoRudder.attach(pinServoRudder);
};

void loop() {
  // Mettre-à-jour les angles d'entrée
  for (int i =0; i<totalInputNumber; i++){
    inputList[i]->updateAngle(); 
  };
  // Mettre-à-jour les angles de sortie
  ManageInputs(inputList, &servoElevator, &servoRudder);
//#ifdef DEBUG_MODE
  delay(20);
//#endif;
};
