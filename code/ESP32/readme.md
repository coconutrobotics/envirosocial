Ο κώδικας για το ESP32 (main.ino) μαζί με τα δοκιμαστικά προγράμματα για τους αισθητήρες.
Αυτή τη στιγμή το πρόγραμμα δουλεύει ως εξής:
1. Περιμένει να λάβει στο bluetooth serial μια εντολή getgata
2. Μόλις λάβει την εντολή υπολογίζει:
2.1. Θερμοκρασία, υγρασία, ατμοσφαιρική πίεση και υψόμετρο από τον BME280
2.2. Συγκέντρωση μικροσωματιδίων από τον dust sensor. Εδώ έκανα μερικές αλλαγές γιατί λογικά παίρνει 3.3V από το ESP και όχι 5V όπως ήταν στο πρόγραμμα
2.3. Ποσοστό επικίνδυνων αερίων % από τον gas sensor. Μέχρι 39% είναι καλή η ατμόσφαιρα
2.4. Ποσοστό θορύβου %

TODO:
1. Να γίνει υλοποίηση και με BLE αντί για bluetooth για χαμηλότερη κατανάλωση.