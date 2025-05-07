# projekt_C
# Specyfikacja projektu: Symulator układania puzzli graficznych

## Cel projektu
Stworzenie aplikacji symulującej układanie puzzli graficznych, umożliwiającej interaktywną manipulację fragmentami obrazka przy pomocy kursora. Program ma zapewnić dynamiczne zarządzanie fragmentami puzzli, zapisywanie i odczytywanie stanu gry oraz pełną obsługę graficznego interfejsu użytkownika.

---

## Wymagania funkcjonalne

### 1. Interfejs użytkownika
- startowe okno z możliwością kontynuowania zapisanej gry lub rozpoczęcia nowej
- Graficzny interfejs umożliwiający wyświetlanie planszy do układania puzzli.
- Możliwość manipulacji fragmentami puzzli przy pomocy kursora (przeciąganie, upuszczanie).
- Przycisk do zapisywania i wczytywania stanu gry.
- Wyraźne komunikaty o stanie gry (np. komunikat o ukończeniu układanki, niepasującym elemencie).

### 2. Manipulowanie fragmentami puzzli
- Program powinien umożliwić wybór, przesuwanie i upuszczanie dowolnego fragmentu obrazka.
- Fragmenty powinny się „przyklejać” do siebie, gdy zostaną połączone poprawnie (opcjonalnie).
- Obsługa kolizji fragmentów puzzli, aby uniemożliwić ich nakładanie się w nieprawidłowy sposób.

### 3. Dynamiczne struktury danych
- Fragmenty puzzli będą przechowywane w dynamicznej strukturze danych, pozwalającej na efektywne dodawanie, usuwanie i modyfikowanie ich pozycji.
- Każdy fragment powinien posiadać informacje o swojej pozycji początkowej(gdzie wraca w razie błednego dopasowania), i docelowej(na właściwej planszy).

### 4. Zarządzanie stanem gry
- Możliwość zapisu aktualnej konfiguracji puzzli do pliku (format rozszerzalny, np. JSON, XML).
- Możliwość wczytania zapisanego stanu gry z pliku, celem kontynuowania rozgrywki.
- Obsługa błędów przy zapisie/odczycie, np. plik uszkodzony, niepoprawny format.

---

## Wymagania niefunkcjonalne
- Aplikacja powinna działać płynnie, z responsywną obsługą kursora.
- Intuicyjny i przejrzysty interfejs graficzny, dostosowany również do użytkowników początkujących.
- Program powinien być odporny na błędy użytkownika, z odpowiednimi komunikatami.

---

Techniczne szczegóły i założenia

- Wybór technologii: Visual Studio Code, jezyk C, biblioteka graficzna SDL.
- Interfejs graficzny powinien wykorzystywać mechanizmy do obsługi zdarzeń myszy.
- Struktury danych:
  - Każdy fragment puzzli to obiekt zawierający informacje o położeniu, wymiarach i grafice.
  - Dynamiczna lista lub inna struktura do przechowywania fragmentów.
  - Struktura przochowująca ułożone i nie ułożone części układanki(właściwa plansza i "pudełko")
- Format zapisu stanu gry to np. JSON, zawierający listę fragmentów z ich pozycjami i parametrami.

---

Planowane kroki implementacji na podstawie milestonów

---

Podsumowanie

Projekt ma na celu stworzenie intuicyjnego i efektownego symulatora puzzli, który pozwoli użytkownikowi na łatwą manipulację fragmentami obrazka oraz zarządzanie stanem gry. Nacisk będzie położony na płynność interakcji i łatwość użytkowania