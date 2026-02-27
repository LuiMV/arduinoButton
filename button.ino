int rojo = 11;
int amarillo = 12;
int verde = 13;
int boton = 2;
int bpush =0;
int modo = 5;

unsigned long tiempoAnterior = 0;
unsigned long ultimoTiempoBoton = 0;

bool estadoBoton = HIGH;
bool ultimoEstadoBoton = HIGH;
bool estadoParpadeo = false;

void setup() {
  pinMode(rojo, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(boton, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  unsigned long tiempoActual = millis();

  //LÓGICA DEL BOTÓN 
  bool lectura = digitalRead(boton);
  if (lectura != ultimoEstadoBoton) {
    ultimoTiempoBoton = tiempoActual;
  }

  if ((tiempoActual - ultimoTiempoBoton) > 50) {
    if (lectura != estadoBoton) {
      estadoBoton = lectura;
      if (estadoBoton == LOW) { 
        modo++;
        if (modo > 5) modo = 1;
        // Limpiar estados al cambiar de modo
        digitalWrite(rojo, LOW);
        digitalWrite(amarillo, LOW);
        digitalWrite(verde, LOW);
      }
    }
  }
  ultimoEstadoBoton = lectura;

  
  switch (modo) {
    case 1: // [1] Solo luz verde
      bpush=1;
      Serial.println(bpush);
      digitalWrite(verde, HIGH);
      digitalWrite(amarillo, LOW);
      digitalWrite(rojo, LOW);
      break;

    case 2: // [2] Todas las luces parpadeando
      if (tiempoActual - tiempoAnterior >= 500) {
        bpush=2;
        Serial.println(bpush);
        tiempoAnterior = tiempoActual;
        estadoParpadeo = !estadoParpadeo;
        digitalWrite(verde, estadoParpadeo);
        digitalWrite(amarillo, estadoParpadeo);
        digitalWrite(rojo, estadoParpadeo);
        
      }
      break;

    case 3: // [3] Solo luz amarilla parpadeando
      digitalWrite(verde, LOW);
      digitalWrite(rojo, LOW);
      if (tiempoActual - tiempoAnterior >= 500) {
        bpush=3;
        Serial.println(bpush);
        tiempoAnterior = tiempoActual;
        estadoParpadeo = !estadoParpadeo;
        digitalWrite(amarillo, estadoParpadeo);
        
      }
      break;

    case 4: // [4] Todas las luces apagadas
      bpush=4;
      Serial.println(bpush);
      digitalWrite(verde, LOW);
      digitalWrite(amarillo, LOW);
      digitalWrite(rojo, LOW);
      
      break;

    case 5: // [5] Modo Semáforo (Ciclo continuo)
      bpush=5;
      Serial.println("Normal boot");
      unsigned long cicloSemaforo = tiempoActual % 6000; 
      if (cicloSemaforo < 2500) { 
        digitalWrite(verde, HIGH); digitalWrite(amarillo, LOW); digitalWrite(rojo, LOW);
      } else if (cicloSemaforo < 3500) { 
        digitalWrite(verde, LOW); digitalWrite(amarillo, HIGH); digitalWrite(rojo, LOW);
      } else { 
        digitalWrite(verde, LOW); digitalWrite(amarillo, LOW); digitalWrite(rojo, HIGH);
      }
      break;
  }
}
