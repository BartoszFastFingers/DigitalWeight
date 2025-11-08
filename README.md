## Co bym poprawił

Projekt powstał trochę w pośpiechu, więc mam świadomość kilku rzeczy, które dałoby się zrobić lepiej:
 
1. **Struktura** – fasada siedzi teraz na wyświetlaczu, a powinna być osobnym modułem. To największy grzech projektu.
2. **Magic numbers** – część wartości mogłaby być opisana stałymi, żeby kod był czytelniejszy.  
3. **Porównania w if-ach** – lepiej najpierw pisać stałe, wtedy kompilator szybciej wychwyci błędy.  
4. **Zarządzanie pamięcią** – przydałoby się parę `free()`, żeby mikrokontroler nie puchł z czasem.  
##
Ogólnie projekt to bardziej **proof of concept**, który pokazał, że pomysł działa — gdybym miał więcej czasu, dopracowałbym architekturę i detale.
