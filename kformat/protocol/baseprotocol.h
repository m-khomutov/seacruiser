/*!
     \file baseprotocol.h
     \author mkh
     \date 25 января 2023 г.
     \brief Заголовочный файл базового класса сетевого протокола трансляции видеокадров.

     Данный файл содержит в себе состояние базового класса сетевого протокола трансляции видеокадров, объявление его интерфейсов.
 */

#ifndef BASEPROTOCOL_H
#define BASEPROTOCOL_H

#include <stdexcept>
#include <vector>

/*!
   \class TProtocolError
   \brief Класс ошибки протокола трансляции видео

   Производный от std::runtime_error. Собственного состояния и методов не содержит
 */
class TProtocolError: public std::runtime_error
{
public:
    /*!
     * \brief Конструктор класса ошибки протокола трансляции видео
     * \param what строковое описание ошибки
     */
    TProtocolError( const std::string & what );
};

class basescreen;

/*!
     \class TBaseprotocol
     \brief Базовый класс сетевого протокола трансляции видеокадров.

      Состояние класса хранит:
      - файловый дескриптор (сокет) сетевого соединения с абонентом;
      - флаги, выставляемые при выдаче видеокадров в сеть;

      Виртуальный публичный интерфейс определяет методы:
      - обработки принятых из сети данных (запрос на выдачу видоепотока);
      - доотправки абоненту не полностью отправленного кадра;
      - отправки видеокадра абоненту;
      - подтверждения возможности отправлять видеокадры;
      - отправки абоненту ошибки (невозможность трансляции при неверном запросе).
      - выдачи номера точки обзора, соответствующей протоколу
 */
class TBaseprotocol {
public:
    /*!
       \brief Фабричный метод создания протокола трансляции видео в соответствии с запросом от абонента
       \param screen указатель на объект отображения сцен
       \param request http запрос трансляции видеопотока
       \param sock файловый дескриптор (сокет) сетевого соединения с абонентом
       \param flags флаги, выставляемые при выдаче данных в сеть
       \return Указатель на объект протокола, созданный в соответствии с полученным запросом
     */
    static TBaseprotocol * create( basescreen *screen, const std::string &request, int sock, int flags );
    /*!
       \brief Конструктор базового класса сетевого протокола трансляции видеокадров.
       \param sock файловый дескриптор (сокет) сетевого соединения с абонентом
       \param flags флаги, выставляемые при выдаче данных в сеть

       Настраивает состояние, а именно сохраняет файловый дескриптор (сокет) сетевого соединения и флаги, выставляемые при выдаче данных в сеть
     */
    explicit TBaseprotocol( int sock, int flags );
    /*!
       \brief Запрещенный конструктор копии.
       \param orig Копируемый объект
     */
    TBaseprotocol(const TBaseprotocol& orig) = delete;
    /*!
       \brief Запрещенный оператор присваивания.
       \param orig Копируемый объект
     */
    TBaseprotocol &operator =(const TBaseprotocol& orig) = delete;
    /*!
       \brief Виртуальный деструктор базового класса сетевого протокола трансляции видеокадров.
     */
    virtual ~TBaseprotocol();
    /*!
     * \brief Вызывается при получении данных из сети.
     * \param data Указатель на буфер данных, принятые из сети
     * \param size Размер данных, принятых из сети
     */
    virtual void on_data( const uint8_t * data, int size ) = 0;
    /*!
     * \brief Позволяет доотправить данные, не ушедшие сразу, вследствие перегрузки сетевых буферов
     */
    virtual void do_write() = 0;
    /*!
     * \brief Отправляет видеокадр абоненту
     * \param data Указатель на отправляемый буфер данных
     * \param size Размер отправляемых данных
     */
    virtual void send_frame( const uint8_t * data, int size ) = 0;
    /*!
     * \brief Подтверждает/Опровергает возможности отправлять видеокадры (контрольный протокол кадры слать не умеет)
     * \return подтверждение/опровержение
     */
    virtual bool can_send_frame() const = 0;
    /*!
     * \brief Отправляет абоненту ошибку (невозможность трансляции при неверном запросе)
     */
    virtual void write_error() = 0;
    /*!
     * \brief Выдает номер точки обзора, соответствующей протоколу
     * \return номер точки обзора
     */
    virtual size_t view() const
    {
        return 0;
    }

protected:
    //! файловый дескриптор (сокет) сетевого соединения с абонентом;
    int fd_;
    //! флаги, выставляемые при выдаче видеокадров в сеть;
    int flags_;
};

/*!
   \class THttpParameter
 * \brief структура, представляющая параметр запроса протокола http
 */
struct THttpParameter
{
    //! имя параметра
    std::string field;
    //! значение параметра
    std::string value;

    /*!
     * \brief Конструктор параметра, заполняющий имя и значение параметра из строки запроса
     * \param requst http запрос
     * \param begin позиция в запросе начала параметра
     * \param end позиция в запросе конца параметра
     */
    THttpParameter( const std::string &requst, size_t begin, size_t end );
    /*!
     * \brief статический метод полного разбора строки запроса и получения всех параметров запроса
     * \param query http запрос
     * \return вектор параметров http-запроса
     */
    static std::vector< THttpParameter > parse( const std::string &query );
};

#endif /* BASEPROTOCOL_H */

