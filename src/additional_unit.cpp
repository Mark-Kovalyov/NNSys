/*
   Copyright 2013-2014 Бондаренко Иван Юрьевич

   Этот файл — часть NNSys.

   NNSys - свободная программа: вы можете перераспространять её и/или изменять
её на условиях Стандартной общественной лицензии GNU в том виде, в каком она
была опубликована Фондом свободного программного обеспечения; либо версии 3
лицензии, либо (по вашему выбору) любой более поздней версии.

   NNSys распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ
ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ
ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.

   Вы должны были получить копию Стандартной общественной лицензии GNU вместе с
этой программой. Если это не так, см. http://www.gnu.org/licenses/gpl.html.
*/

#include <fstream>
#include <iostream>

#include "additional_unit.h"

EIncorrectArg::EIncorrectArg(int iArg) throw()
{
    m_sErrorMsg = QString::number(iArg)
            + QString("-й аргумент командной строки задан неверно.");
}

const char* EIncorrectArg::what() const throw()
{
    return qPrintable(m_sErrorMsg);
}

/* Сравнение строк szText1 и szText2 без учёта регистра символов. Если строки
равны, то вовзращается true, если же не равны - false. */
bool same_text_nocase(const char *szText1, const char *szText2)
{
    bool result = true;
    int i = 0;
    char line_end = 0;
    while ((szText1[i] != line_end) && (szText2[i] != line_end))
    {
        if (toupper(szText1[i]) == toupper(szText2[i]))
        {
            i++;
        }
        else
        {
            result = false;
            break;
        }
    }
    if (result)
    {
        if (!((szText1[i] == line_end) && (szText2[i] == line_end)))
        {
            result = false;
        }
    }
    return result;
}

/* Преобразование строки sSrc в вещественное число rDst типа float. В случае
успешного преобразования возвращается true, в случае ошибки - false. */
bool string_to_real(const std::string& sSrc, float& rDst)
{
    bool result = true;
    try
    {
        std::istringstream stream_for_input(sSrc);
        stream_for_input >> rDst;
        if ((!stream_for_input.good()) && (!stream_for_input.eof()))
        {
            result = false;
        }
    }
    catch(std::ios_base::failure)
    {
        result = false;
    }
    return result;
}

/* Преобразование строки sSrc в вещественное число rDst типа double. В случае
успешного преобразования возвращается true, в случае ошибки - false. */
bool string_to_real(const std::string& sSrc, double& rDst)
{
    bool result = true;
    try
    {
        std::istringstream stream_for_input(sSrc);
        stream_for_input >> rDst;
        if ((!stream_for_input.good()) && (!stream_for_input.eof()))
        {
            result = false;
        }
    }
    catch(std::ios_base::failure)
    {
        result = false;
    }
    return result;
}

/* Преобразование строки sSrc в вещественное число rDst типа long double.
В случае успешного преобразования возвращается true, в случае ошибки - false.*/
bool string_to_real(const std::string& sSrc, long double& rDst)
{
    bool result = true;
    try
    {
        std::istringstream stream_for_input(sSrc);
        stream_for_input >> rDst;
        if ((!stream_for_input.good()) && (!stream_for_input.eof()))
        {
            result = false;
        }
    }
    catch(std::ios_base::failure)
    {
        result = false;
    }
    return result;
}

/* Преобразование строки sSrc в целое число rDst. В случае успешного
преобразования возвращается true, в случае ошибки - false. */
bool string_to_integer(const std::string& sSrc, int& rDst)
{
    bool result = true;
    try
    {
        std::istringstream stream_for_input(sSrc);
        stream_for_input >> rDst;
        if ((!stream_for_input.good()) && (!stream_for_input.eof()))
        {
            result = false;
        }
    }
    catch(std::ios_base::failure)
    {
        result = false;
    }
    return result;
}

/* Все аргументы командной строки должны иметь вид:
   -<строка-ключ>=<строка-значение>
   или
   -<строка-ключ>
   Данная функция выполняет анализ аргументов командной строки на предмет
соответствия этим шаблонам и добавляет найденные пары "ключ-значение" в словарь
параметров rParams, предварительное преобразовав символы ключа к нижнему
регистру. Кроме того, ни одна строка-ключ не должна повторяться два или более
раз.
   В случае, если вышеописанные условия выполняются и словарь параметров
успешно сформирован, функция срабатывает нормально. Также нормальное срабатывание
происходит, если аргументы командной строки отсутствуют. Остальные случаи
расцениваются как ошибки, и тогда возвращается возбуждается исключение
EIncorrectArg. */
void parse_command_line(int argc, char *argv[], TCmdParams& rParams)
{
    rParams.clear();
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            QString sArgText = QString::fromLocal8Bit(argv[i]);
            QString sKey, sValue;
            if (sArgText.size() <= 1)
            {
                throw EIncorrectArg(i);
            }
            else
            {
                if (sArgText[0] == '-')
                {
                    sArgText.remove(0, 1);
                    int found_pos = sArgText.indexOf('=', 0);
                    if (found_pos < 0)
                    {
                        sKey = sArgText;
                    }
                    else
                    {
                        if (found_pos > 0)
                        {
                            sKey = sArgText.left(found_pos);
                            sValue = sArgText.right(sArgText.size()
                                                    - found_pos - 1);
                        }
                        else
                        {
                            throw EIncorrectArg(i);
                        }
                    }
                    if (rParams.count(sKey) > 0)
                    {
                        throw EIncorrectArg(i);
                    }
                    else
                    {
                        rParams[sKey] = sValue;
                    }
                }
                else
                {
                    throw EIncorrectArg(i);
                }
            }
        }
    }
}

/* Распечатать (вывести в stdout) строку sLine. Если её длина меньше width, то
дополнить справа пробелами. */
void print_line(const QString& sLine, int width)
{
    int n = width - sLine.size();
    if (n > 0)
    {
        for (int i = 0; i < n; i++)
        {
            std::cout << ' ';
        }
    }
    std::cout << qPrintable(sLine);
}

/* Удалить пробельные символы и символы табуляции из начала и из конца строки
sLine. */
void trim_line(QString& sLine)
{
    int i = 0, n = sLine.length();
    while (i < n)
    {
        if (is_space(sLine[i]))
        {
            i++;
        }
        else
        {
            break;
        }
    }
    if (i < n)
    {
        sLine.remove(0, i);
        n = sLine.length();
        i = n - 1;
        while (i >= 0)
        {
            if (is_space(sLine[i]))
            {
                i--;
            }
            else
            {
                break;
            }
        }
        if (i >= 0)
        {
            if (i < (n - 1))
            {
                sLine.remove(i + 1, n - 1 - i);
            }
        }
        else
        {
            sLine.clear();
        }
    }
    else
    {
        sLine.clear();
    }
}

/* Удалить пробельные символы и символы табуляции из начала и из конца строки
sLine. */
void trim_line(std::string& sLine)
{
    int i = 0, n = sLine.length();
    while (i < n)
    {
        if (is_space(sLine[i]))
        {
            i++;
        }
        else
        {
            break;
        }
    }
    if (i < n)
    {
        sLine.erase(0, i);
        n = sLine.length();
        i = n - 1;
        while (i >= 0)
        {
            if (is_space(sLine[i]))
            {
                i--;
            }
            else
            {
                break;
            }
        }
        if (i >= 0)
        {
            if (i < (n - 1))
            {
                sLine.erase(i + 1, n - 1 - i);
            }
        }
        else
        {
            sLine.clear();
        }
    }
    else
    {
        sLine.clear();
    }
}
