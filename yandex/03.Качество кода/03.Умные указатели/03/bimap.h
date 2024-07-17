#pragma once

#include <memory>
#include <optional>
#include <string_view>

class BiMap {
    
    struct Impl;
    std::unique_ptr<Impl> impl_;
    
public:
    
    BiMap();
    ~BiMap();
    
    //Копирующий конструктор
    BiMap(const BiMap& other);
    BiMap& operator=(const BiMap& other);
    
    // Перемещающий конструктор
    BiMap(BiMap&&) noexcept;
    BiMap& operator=(BiMap&&) noexcept;
    
    /**
     * Добавляет в словарь пару "ключ-значение".
     * В случае успеха возвращает true.
     * Возвращает false, если ключ или значение были ранее добавлены.
     * 
     * Метод обеспечивает строгую гарантию безопасности исключений
     */
    bool Add(std::string_view key, std::string_view value);

    /**
     * Возвращает значение, связанное с ключом, либо nullopt, если такого ключа нет
     */
    std::optional<std::string_view> FindValue(std::string_view key) const noexcept;

    /**
     * Возвращает ключ, связанный с value, либо nullopt, если такого значения нет
     */
    std::optional<std::string_view> FindKey(std::string_view value) const noexcept;
};

/** sln
 #pragma once
 #include <memory>
 #include <optional>
 #include <string_view>

 class BiMap {
 public:
     BiMap();

     BiMap(BiMap&& other) noexcept;
     BiMap& operator=(BiMap&& other) noexcept;

     BiMap(const BiMap& other);
     BiMap& operator=(const BiMap& other);

     ~BiMap();

     /**
      * Добавляет в словарь пару "ключ-значение".
      * В случае успеха возвращает true.
      * Возвращает false, если ключ или значение были ранее добавлены.
      *
      * Метод обеспечивает строгую гарантию безопасности исключений
      */
     bool Add(std::string_view key, std::string_view value);

     /**
      * Возвращает значение, связанное с ключом, либо nullopt, если такого ключа нет
      */
     std::optional<std::string_view> FindValue(std::string_view key) const noexcept;

     /**
      * Возвращает ключ, связанный с value, либо nullopt, если такого значения нет
      */
     std::optional<std::string_view> FindKey(std::string_view value) const noexcept;

 private:
     struct Impl;
     std::unique_ptr<Impl> impl_;
 };
 
 */
