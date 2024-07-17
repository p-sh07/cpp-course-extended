#include <cassert>

#include "bimap.h"

int main() {
    using namespace std::literals;

    BiMap bimap;
    {
        const bool cat_added = bimap.Add("Cat"sv, "Koshka"sv);

        assert(cat_added);

        assert(bimap.FindValue("Cat"sv) == "Koshka"sv);
        assert(bimap.FindKey("Koshka"sv) == "Cat"sv);
        assert(!bimap.FindKey("Cat"sv));
        assert(!bimap.FindValue("Koshka"sv));

        const bool kitty_added = bimap.Add("Cat"sv, "Kitty"sv);

        assert(!kitty_added);
        assert(bimap.FindValue("Cat"sv) == "Koshka"sv);
        assert(!bimap.FindValue("Kitty"sv));
    }

    BiMap bimap_copy(bimap);
    assert(bimap_copy.FindValue("Cat"sv) == "Koshka"sv);
    assert(bimap_copy.FindKey("Koshka"sv) == "Cat"sv);

    bimap_copy.Add("Dog"sv, "Sobaka"sv);
    assert(bimap_copy.FindValue("Dog"sv) == "Sobaka"sv);
    assert(bimap_copy.FindKey("Sobaka"sv) == "Dog"sv);
    assert(!bimap.FindKey("Sobaka"sv));
    assert(!bimap.FindValue("Dog"sv));

    bimap = std::move(bimap_copy);
    assert(bimap.FindValue("Dog"sv) == "Sobaka"sv);
    assert(bimap.FindKey("Sobaka"sv) == "Dog"sv);

    BiMap moved_bimap(std::move(bimap));
    assert(moved_bimap.FindValue("Dog"sv) == "Sobaka"sv);
    assert(moved_bimap.FindKey("Sobaka"sv) == "Dog"sv);

    BiMap bimap_to_assign;
    bimap_to_assign = moved_bimap;
    assert(bimap_to_assign.FindValue("Dog"sv) == "Sobaka"sv);
    assert(bimap_to_assign.FindKey("Sobaka"sv) == "Dog"sv);
}

/*
 Примените идиому Pimpl при разработке класса BiMap. Он моделирует двунаправленный словарь, хранящий пары строк ключ-значение. В отличие от map или unordered_map в нём можно быстро искать не только значение по ключу, но и ключ по значению. Поэтому в нём ключи и значения не дублируются. Реализуйте в BiMap три метода:
 Add. Добавляет в словарь пару строк ключ-значение. Возвращает true, если вставка прошла успешно, то есть словарь до вызова метода не содержал таких ключей и значений. Если в словаре уже есть такой ключ или значение, метод возвращает false и оставляет словарь без изменений.
 FindValue. Возвращает значение, соответствующее указанному ключу, либо nullopt, если такого ключа нет.
 FindKey. Возвращает ключ, имеющий указанное значение, либо nullopt, если такого значения нет.
 Сигнатура методов класса:
 class BiMap {
 public:
     bool Add(std::string_view key, std::string_view value);

     std::optional<std::string_view> FindValue(std::string_view key) const noexcept;

     std::optional<std::string_view> FindKey(std::string_view value) const noexcept;
 };
 Реализация должна использовать идиому Pimpl. Чтобы убедиться в этом, тренажёр проверит, что размер класса BiMap не превышает размера указателя.
 Метод Add должен обеспечивать строгую гарантию безопасности исключений, а методы поиска — гарантировать отсутствие исключений.
 Класс BiMap должен поддерживать копирование и перемещение. Экземпляром BiMap, из которого переместили значение, пользоваться нельзя. Попытка использовать вызвать методы приведёт к неопределённому поведению. Если в словарь скопировать или перемести значение другого словаря, то словарём вновь разрешается пользоваться.
 Конструктор копирования и операция копирующего присваивания должны обеспечивать строгую гарантию безопасности исключений. Перемещающий конструктор и операция перемещающего присваивания должны гарантировать отсутствие исключений.
 Подберите такие структуры данных для хранения элементов словаря, чтобы выполнялись требования по производительности:
 Среднее время вставки — константа. В худшем случае линейно зависит от количества элементов.
 Среднее время поиска значения по ключу и ключа по значению — также константа. В худшем случае линейно зависит от количества элементов.
 Конструктор перемещения и операция перемещающего присваивания выполняются за константное время.
 Конструктор копирования и операция копирующего присваивания выполняются за время, линейно зависящее от размера словаря.
 Как будет тестироваться ваш код
 Тренажёр проверит, что:
 размер класса BiMap не превышает размера класса с единственным полем типа unique_ptr;
 методы работают в соответствии с заданием;

 */
