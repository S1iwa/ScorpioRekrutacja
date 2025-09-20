# Projekt Scorpio - zadanie rekrutacyjne do działu Software

>**Uwaga!** Przed przystąpieniem do realizacji zadania przeczytaj **całe** README.

## Spis treści
- [Informacje ogólne](#informacje-ogólne)
- [Zadania do wykonania](#zadania-do-wykonania)
- [Specyfikacja techniczna zadania](#specyfikacja-techniczna-zadania)
  - [Symulacja](#symulacja)
  - [Dokumentacja API](#dokumentacja-api)
  - [Dokumentacja argumentów CLI](#dokumentacja-argumentów-cli)
- [Wskazówki i przydatne linki](#wskazówki-i-przydatne-linki)

## Informacje ogólne
Zadanie wykorzystuje symulację dwu-osiowej platformy obrotowej, w której za pomocą stworzonego przez nas API można sterować dwoma silnikami oraz dokonywać odczytu wartości ich enkoderów. Silnik numer 1 jest odpowiedzialny za ruch w osi poziomej, zaś numer 2 w pionowej.

Uznajemy, na potrzebę zadań, że na osi pionowej zamontowana jest kamera, którą możemy się rozglądać sterując silnikami.

Całość zadania należy wykonać w języku C++, zgodnie ze standardem C++17.

## Zadania do wykonania 
W tej części znajdziesz treść poszczególnych zadań, szczegółowe informacje dotyczące symulacji oraz jej implementacji znajdziesz w [specyfikacja techniczna zadania](#specyfikacja-techniczna-zadania).

Pamiętaj, że zadanie służy sprawdzeniu wielu umiejętności - nie tylko programowania i znajomości algorytmów -  więc nawet w przypadku zrealizowania tylko części z poniższych punktów, zachęcamy do przesłania rozwiązania. Postępy w zadaniu powinny być udokumentowane w repozytorium na githubie (po każdym etapie zadania powinien zostać stworzony nowy commit).

1. **Zbudowanie projektu:**

- Sklonuj to repozytorium, skonfiguruj środowisko i upewnij się, że projekt kompiluje się przy użyciu `CMake`.
- Uruchom skompilowany plik wykonywalny i zadaj cel za pomocą wiersza poleceń, np: `5 3 4 5`.
- Za pomocą `Shift+D` zakończ wprowadzanie celów. Program powinien zakończyć działanie, po implementacji rozwiązania w tym momencie zostaną mu zadane wprowadzone cele.

> **Wskazówka!** Dobrym rozwiązaniem jest "fork" tego repozytorium

2. **Dojazd do pojedyńczego celu**

- Poprzez komponent odpowiedzialny za zadawanie punktów zostanie przesłany pojedyńczy punkt. Należy obrócić obydwoma silnikami tak, aby kamera patrzyła na ten punkt.

> **Wskazówka!** W sekcji [symulacja](#symulacja) znajdziesz dokładny opis platformy obrotowej.

3. **Dojazd do wielu celów (priorytet ostatniego)**

- Poprzez komponent odpowiedzialny za zadawanie punktów zostanie przesłana arbitralna liczba punktów w odstępach niewiększych niż 10 sekund. Każdy punkt należy obsłużyć dokładnie tak jak w poprzednim zadaniu. Jeżeli podczas dojazdu do celu zostanie zadany kolejny cel, należy obecny cel porzucić.

> **Wskazówka!** Pamiętaj o regularnym commitowaniu rozwiązania.

4.1. **Dojazd do wielu celów (kolejka)**

- Poprzez komponent odpowiedzialny za zadawanie punktów zostanie przesłana arbitralna liczba punktów w odstępach niewiększych niż 10 sekund. Należy każdy z tych punktów obsłużyć dokładnie tak jak w poprzednim zadaniu, dojeżdżając do każdego po kolei, bez wywłaszczenia.

4.2. **Dojazd do wielu celów (z ograniczeniami)**

- W tym zadaniu zostaną zadane limity dla każdego z silników. Cele należy obsłużyć analogicznie do zadania 4.1 (kolejkowo) i dojazd do nich może być niemożliwy w ramach zadanych limitów.

## Specyfikacja techniczna zadania

### Symulacja

#### System współrzędnych i mapowanie enkoderów

Symulacja implementuje dwu-osiową platformę obrotową z następującym układem współrzędnych:

**Kamera**
- Znajduje się i obraca w punkcie `(0,0,0)`, czyli nie symulujemy żadnych wymiarów montażu, samej kamery, etc.

**Silnik 1 - Oś pozioma:**
- Wartość = 0 → kamera skierowana w kierunku +X
- Wartość rosnąca → silnik kręci się zgodnie z ruchem wskazówek zegara

**Silnik 2 - Oś pionowa:**
- Wartość = 0 → kamera skierowana "poziomo" (równolegle do płaszczyzny XY)
- Wartość rosnąca → silnik kręci się do góry

#### Charakterystyka ruchu

**Sygnały sterujące:**
- Wartości dodatnie (1-127): ruch w kierunku rosnących wartości enkodera
- Wartości ujemne (-128 do -1): ruch w kierunku malejących wartości enkodera
- Wartość 0: brak ruchu

**Właściwości enkoderów:**
- Zakres: 0-4095 (12-bit, cykliczny)
- Rozdzielczość: 4096 pozycji na pełny obrót (360°)
- Szum: dodawany szum gaussowski (σ = 0.6)
- Częstotliwość odczytu: konfigurowalna (domyślnie 20 Hz)

**Ograniczenia mechaniczne:**
- Silniki respektują limity ustawione przez parametry CLI (`-t`, `-d`, `-l`, `-r`)
- Po osiągnięciu limitu silnik zatrzymuje się na pozycji granicznej
- Enkodery są cykliczne: po wartości 4095 następuje 0

**Uwagi implementacyjne:**
- Prędkość ruchu jest proporcjonalna do kwadratu sygnału sterującego
- Silniki mają bezwładność - nie zatrzymują się natychmiast
- Callback enkodera jest wywoływany co okres aktualizacji z dodanym szumem

### Dokumentacja API

> **Uwaga!** Nie należy modyfikować implementacji symulacji. Całe twoje rozwiązanie powinno znaleźć się w katalogach `*/solution`, a punktem wejściowym twojego rozwiązania powinna być metoda `solver()` w pliku `solution.cpp`.

#### Interfejs Tester

Interfejs `backend_interface::Tester` zapewnia dostęp do symulacji dwu-osiowej platformy obrotowej. Pozwala na sterowanie silnikami oraz odbieranie sygnałów poleceń.

**Składniki:**
- `get_motor_1()` - zwraca silnik osi poziomej
- `get_motor_2()` - zwraca silnik osi pionowej
- `get_commands()` - zwraca komponent do odbierania poleceń pozycji

#### Interfejs Component

Wszystkie komponenty implementują interfejs `Component<Send, Receive>`, który udostępnia możliwość pracy w modelu publisher/subscriber:

```cpp
template <typename Send, typename Receive>
class Component {
public:
  virtual void add_data_callback(std::function<void(const Receive&)>) = 0;
  virtual void send_data(const Send&) = 0;
};
```

#### Silniki (`Component<int8_t, uint16_t>`)

**Wysyłanie sygnału sterującego:**
```cpp
auto motor1 = tester->get_motor_1();
motor1->send_data(100);  // Wartość z zakresu int8_t
```

**Odbieranie pozycji enkodera:**
```cpp
motor1->add_data_callback([](const uint16_t& encoder_value) {
    std::cout << "Pozycja enkodera: " << encoder_value << std::endl;
});
```

**Charakterystyka silników:**
- Typ sygnału sterującego: `int8_t` (zakres: -128 do 127)
- Typ odczytu enkodera: `uint16_t` (zakres: 0 do 4095)
- Motor 1: steruje osią poziomą
- Motor 2: steruje osią pionową
- Ograniczenia: silniki respektują limity ustawione podczas inicjalizacji

#### Polecenia (`Component<Impossible, Point>`)

**Odbieranie poleceń pozycji:**
```cpp
auto commands = tester->get_commands();
commands->add_data_callback([](const Point& target) {
  std::cout << "Cel: x=" << target.x 
            << " y=" << target.y 
            << " z=" << target.z << std::endl;
});
```

**Struktura Point:**

Reprezentuje punkt w przestrzeni trójwymiarowej.

```cpp
struct Point {
  double x;  // Współrzędna X
  double y;  // Współrzędna Y  
  double z;  // Współrzędna Z
};
```

**Struktura Constraints:**

Struktura określna maksymalne/minimalne wartości odczytu z enkoderów silników.

```cpp
struct Constraints {
  std::optional<uint16_t> vertical_max;   // Ograniczenie górne osi pionowej.
  std::optional<uint16_t> vertical_min;   // Ograniczenie dolne osi pionowej
  std::optional<uint16_t> horizontal_max; // Ograniczenie górne osi poziomej
  std::optional<uint16_t> horizontal_min; // Ograniczenie dolne osi poziomej
};
```

**Uwagi:**
- Polecenia są wysyłane automatycznie przez symulator zgodnie z danymi wejściowymi
- Typ `Impossible` uniemożliwia wysyłanie danych do komponentu poleceń

### Dokumentacja argumentów CLI

Program symulacji platformy obrotowej obsługuje następujące argumenty wiersza poleceń:

#### Składnia
```
./scorpio_recruitment_task [opcje]
```

#### Dostępne opcje

| Opcja | Argument | Opis |
|-------|----------|------|
| `-h` | - | Wyświetla wiadomość pomocy z listą wszystkich dostępnych opcji |
| `-f` | FILE | Odczytuje dane wejściowe z podanego pliku zamiast ze standardowego wejścia |
| `-g` | - | Włącza tryb debugowania (verbose output) |
| `-t` | LIMIT | Ustawia maksymalny (górny) limit odczytu enkodera przy ruchu silnika pionowego w górę |
| `-d` | LIMIT | Ustawia minimalny (dolny) limit odczytu enkodera przy ruchu silnika pionowego w dół |
| `-r` | LIMIT | Ustawia maksymalny (prawy) limit odczytu enkodera przy ruchu silnika poziomego w prawo |
| `-l` | LIMIT | Ustawia minimalny (lewy) limit odczytu enkodera przy ruchu silnika poziomego w lewo |
| `-q` | PERIOD | Ustawia okres aktualizacji enkoderów (w sekundach) |

#### Przykłady użycia

```bash
# Uruchomienie z domyślnymi ustawieniami
./scorpio_recruitment_task

# Uruchomienie z plikiem wejściowym
./scorpio_recruitment_task -f commands.txt

# Uruchomienie z niestandardowymi limitami i trybem debug
./scorpio_recruitment_task -g -t 3000 -d 1000 -l 3500 -r 500

# Uruchomienie z szybszym odświeżaniem enkoderów
./scorpio_recruitment_task -q 0.01

# Kombinacja opcji
./scorpio_recruitment_task -f input.txt -g -q 0.02 -t 4000 -d 0 -l 4095 -r 0
```

#### Uwagi
- Argumenty można podawać w dowolnej kolejności
- W przypadku błędnych argumentów program wyświetli komunikat o błędzie i zakończy działanie
- Użyj opcji `-h` aby wyświetlić szybką pomoc

## Wskazówki i przydatne linki

- [C++17 Reference - cppreference.com](https://en.cppreference.com/)
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- Zadanie rekrutacyjne można oddać niepełne.
- Rozwiązane zadanie należy umieścić w **publicznym** repozytorium (np. GitHub) i przesłać linka do tego repozytorium na mail projekt@scorpio.pwr.edu.pl. Ewentualne pytania lub wątpliwości co do treści zadania można kierować na tego samego maila. Zadania przyjmujemy do 06.04.2025 do końca dnia.
//  TODO(@mariusz): Change this date

**Jeżeli będziesz miał jakiekolwiek wątpliwości i problemy z zadaniem śmiało skontaktuj się z nami na maila projekt@scorpio.pwr.edu.pl! Powodzenia :)**
