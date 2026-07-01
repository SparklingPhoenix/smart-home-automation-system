import telebot
import mysql.connector 
import serial
import time
import re
from telebot import types
from time import sleep
         
TOKEN = "1234567890:ABCDEFGHIJKLMNOPQRSTUV-ABCDEFGHIJK0"
bot = telebot.TeleBot(TOKEN)
CHANNEL_NAME = "-0987654321"
Menu = int()
My_string = ""
guard_button_1 = ""
guard_button_2 = ""
therm_button_1 = ""
therm_button_2 = ""
therm_button_3 = ""
therm_button_4 = ""
therm_button_5 = ""
therm_button_6 = ""
therm_button_7 = ""
therm_button_8 = ""
btn_2 = ""
btn_3 = ""
btn_4 = ""
btn_5 = ""
btn_6 = ""
btn_7 = ""
btn_8 = ""
btn_9 = ""
btn_10 = ""
btn_11 = ""
btn_12 = ""
msg_id = int()

@bot.message_handler(commands=['start'])
def main_menu(message):
    global Menu
    Menu = 0   
    bot.delete_message(CHANNEL_NAME, message.message_id)
    main_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
    main_menu_btn_1 = telebot.types.KeyboardButton(text='Мікроклімат')
    main_menu_btn_2 = telebot.types.KeyboardButton(text='Енерго-аудит')
    main_menu_btn_3 = telebot.types.KeyboardButton(text='Охорона')
    main_menu_btn_4 = telebot.types.KeyboardButton(text='Освітлення')
    main_menu_keyboard.add(main_menu_btn_1, main_menu_btn_2)
    main_menu_keyboard.add(main_menu_btn_3, main_menu_btn_4)
    bot.send_message(CHANNEL_NAME, 'Головне Меню', reply_markup=main_menu_keyboard)

@bot.message_handler(content_types=['text'])
def handler(message):
    global Menu
    global msg_id
    global My_string
    global guard_button_1
    global guard_button_2
    global therm_button_1
    global therm_button_2
    global therm_button_3
    global therm_button_4   
    global therm_button_5
    global therm_button_6
    global therm_button_7
    global therm_button_8
    global btn_2
    global btn_3
    global btn_4
    global btn_5
    global btn_6
    global btn_7
    global btn_8
    global btn_9
    global btn_10
    global btn_11
    global btn_12
    if message.text == "Головне меню":
        main_menu(message)
        bot.delete_message(CHANNEL_NAME, message.message_id - 1)
    elif (message.text == "Мікроклімат") or (Menu == 1 and message.text == 'Оновити дані'):
        Menu = 1        
        bot.delete_message(CHANNEL_NAME, message.message_id)
        SQL_request_therm_data()
        therm_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
        therm_button_1 = telebot.types.KeyboardButton(text='Газовий котел')
        therm_button_2 = telebot.types.KeyboardButton(text='Кондиціонер')
        main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
        refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
        therm_menu_keyboard.add(therm_button_1, therm_button_2)
        therm_menu_keyboard.add(main_menu_btn, refresh_therm_btn)
        bot.send_message(CHANNEL_NAME, My_string, reply_markup=therm_menu_keyboard)
        bot.delete_message(CHANNEL_NAME, message.message_id - 1)
    elif (message.text == "Газовий котел") or (message.text == "Назад" and Menu == 111) or (message.text == "Назад" and Menu == 112):
        Menu = 11
        bot.delete_message(CHANNEL_NAME, message.message_id)
        SQL_request_therm_boiler_data()
        therm_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
        if My_string.find('Базовий термостат 🔥: on')>-1:
            therm_button_1 = telebot.types.KeyboardButton(text='Вимкнути котел 🔥')
        if My_string.find('Базовий термостат 🔥: off')>-1:
            therm_button_1 = telebot.types.KeyboardButton(text='Запустити котел 🔥')
        therm_button_2 = telebot.types.KeyboardButton(text='Базова уставка °С')    
        if My_string.find('Еко-термостат (охорона) 🔥: on')>-1:
            therm_button_3 = telebot.types.KeyboardButton(text='Вимкнути "еко-режим" 🔥')
        if My_string.find('Еко-термостат (охорона) 🔥: off')>-1:
            therm_button_3 = telebot.types.KeyboardButton(text='Активувати "еко-режим" 🔥')
        therm_button_4 = telebot.types.KeyboardButton(text='Еко-уставка °С')        
        refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
        back_btn = telebot.types.KeyboardButton(text='Назад')
        main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
        therm_menu_keyboard.add(therm_button_1, therm_button_2)
        therm_menu_keyboard.add(therm_button_3, therm_button_4)
        therm_menu_keyboard.add(back_btn, refresh_therm_btn)
        therm_menu_keyboard.add(main_menu_btn)
        bot.send_message(CHANNEL_NAME, My_string, reply_markup=therm_menu_keyboard)
        bot.delete_message(CHANNEL_NAME, message.message_id - 1)
    elif (message.text == "Кондиціонер") or (message.text == "Назад" and Menu == 121) or (message.text == "Назад" and Menu == 122):
        Menu = 12        
        bot.delete_message(CHANNEL_NAME, message.message_id)
        SQL_request_therm_AC_data()
        therm_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
        if My_string.find('Базовий термостат 🥶: on')>-1:
            therm_button_5 = telebot.types.KeyboardButton(text='Вимкнути кондиціонер 🥶')
        if My_string.find('Базовий термостат 🥶: off')>-1:
            therm_button_5 = telebot.types.KeyboardButton(text='Запустити кондиціонер 🥶')
        therm_button_6 = telebot.types.KeyboardButton(text='Базова уставка °С')
        if My_string.find('Еко-термостат (охорона) 🥶: on')>-1:
            therm_button_7 = telebot.types.KeyboardButton(text='Вимкнути "еко-режим" 🥶')
        if My_string.find('Еко-термостат (охорона) 🥶: off')>-1:
            therm_button_7 = telebot.types.KeyboardButton(text='Активувати "еко-режим" 🥶')
        therm_button_8 = telebot.types.KeyboardButton(text='Еко-уставка °С')        
        refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
        back_btn = telebot.types.KeyboardButton(text='Назад')
        main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
        therm_menu_keyboard.add(therm_button_5, therm_button_6)
        therm_menu_keyboard.add(therm_button_7, therm_button_8)
        therm_menu_keyboard.add(back_btn, refresh_therm_btn)
        therm_menu_keyboard.add(main_menu_btn)
        bot.send_message(CHANNEL_NAME, My_string, reply_markup=therm_menu_keyboard)
        bot.delete_message(CHANNEL_NAME, message.message_id - 1)
    elif (message.text == "Енерго-аудит") or (Menu == 2 and message.text == 'Оновити дані'):
        Menu = 2         
        bot.delete_message(CHANNEL_NAME, message.message_id)
        SQL_request_power_data()
        power_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
        main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
        refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')        
        power_menu_keyboard.add(main_menu_btn, refresh_therm_btn)      
        bot.send_message(CHANNEL_NAME, My_string, reply_markup=power_menu_keyboard)
        bot.delete_message(CHANNEL_NAME, message.message_id - 1)
    elif (message.text == "Охорона") or (Menu == 3 and message.text == 'Оновити дані'):
        Menu = 3         
        SQL_request_guard_data()  
        bot.delete_message(CHANNEL_NAME, message.message_id)
        guard_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
        if My_string.find('Охорона: on 👮‍♂️')>-1:
            guard_button_1 = telebot.types.KeyboardButton(text='Вимкнути охорону')
        if My_string.find('Охорона: off 😴')>-1:
            guard_button_1 = telebot.types.KeyboardButton(text='Активувати охорону')
        if My_string.find('Тривога: on 🔔')>-1:
            guard_button_2 = telebot.types.KeyboardButton(text='Вимкнути тривогу')
        if My_string.find('Тривога: off 🔕')>-1:
            guard_button_2 = telebot.types.KeyboardButton(text='Активувати тривогу')
        main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
        refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
        guard_menu_keyboard.add(guard_button_1, guard_button_2)
        guard_menu_keyboard.add(main_menu_btn, refresh_therm_btn)      
        bot.send_message(CHANNEL_NAME, My_string, reply_markup=guard_menu_keyboard)
        bot.delete_message(CHANNEL_NAME, message.message_id - 1)
    elif message.text == "Освітлення"  or (Menu == 4 and message.text == 'Оновити дані'):
        Menu = 4         
        bot.delete_message(CHANNEL_NAME, message.message_id, timeout = 1)
        lightening_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
        SQL_request_lightening_data()
        btn_1 = types.InlineKeyboardButton(text="Kill'em All!")
        if My_string.find('Передпокій: off')>-1:
            btn_2 = telebot.types.KeyboardButton(text='Передпокій')
        if My_string.find('Передпокій: on')>-1:
            btn_2 = telebot.types.KeyboardButton(text='Передпокій💡')
        if My_string.find('Спальня: off')>-1:
            btn_3 = telebot.types.KeyboardButton(text='Спальня')
        if My_string.find('Спальня: on')>-1:
            btn_3 = telebot.types.KeyboardButton(text='Спальня💡')        
        if My_string.find('Вбиральня: off')>-1:
            btn_4 = telebot.types.KeyboardButton(text='Вбиральня')
        if My_string.find('Вбиральня: on')>-1:
            btn_4 = telebot.types.KeyboardButton(text='Вбиральня💡')        
        if My_string.find('Сан. вузол: off')>-1:
            btn_5 = telebot.types.KeyboardButton(text='Сан. вузол')
        if My_string.find('Сан. вузол: on')>-1:
            btn_5 = telebot.types.KeyboardButton(text='Сан. вузол💡')
        if My_string.find('Вітальня: off')>-1:
            btn_6 = telebot.types.KeyboardButton(text='Вітальня')
        if My_string.find('Вітальня: on')>-1:
            btn_6 = telebot.types.KeyboardButton(text='Вітальня💡')
        if My_string.find('Кухня: off')>-1:
            btn_7 = telebot.types.KeyboardButton(text='Кухня')
        if My_string.find('Кухня: on')>-1:
            btn_7 = telebot.types.KeyboardButton(text='Кухня💡')
        if My_string.find('Кабінет: off')>-1:
            btn_8 = telebot.types.KeyboardButton(text='Кабінет')
        if My_string.find('Кабінет: on')>-1:
            btn_8 = telebot.types.KeyboardButton(text='Кабінет💡')
        if My_string.find('Витяжка у сан-вузлу: off')>-1:
            btn_9 = telebot.types.KeyboardButton(text='Витяжка у сан-вузлу')
        if My_string.find('Витяжка у сан-вузлу: on')>-1:
            btn_9 = telebot.types.KeyboardButton(text='Витяжка у сан-вузлу 💨')
        if My_string.find('Витяжка у спальні: off')>-1:
            btn_10 = telebot.types.KeyboardButton(text='Витяжка у спальні')
        if My_string.find('Витяжка у спальні: on')>-1:
            btn_10 = telebot.types.KeyboardButton(text='Витяжка у спальні 💨')
        if My_string.find('Amplifier: off')>-1:
            btn_11 = telebot.types.KeyboardButton(text='Amplifier')
        if My_string.find('Amplifier: on')>-1:
            btn_11 = telebot.types.KeyboardButton(text='Amplifier 🔊')
        if My_string.find('Bluetooth: off')>-1:
            btn_12 = telebot.types.KeyboardButton(text='Bluetooth')
        if My_string.find('Bluetooth: on')>-1:
            btn_12 = telebot.types.KeyboardButton(text='Bluetooth 📡')
        refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
        main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
        lightening_menu_keyboard.add(btn_1, btn_2)
        lightening_menu_keyboard.add(btn_3, btn_4)
        lightening_menu_keyboard.add(btn_5, btn_6)
        lightening_menu_keyboard.add(btn_7, btn_8)
        lightening_menu_keyboard.add(btn_9, btn_10)
        lightening_menu_keyboard.add(btn_11, btn_12)
        lightening_menu_keyboard.add(main_menu_btn, refresh_therm_btn) 
        bot.send_message(CHANNEL_NAME, My_string, reply_markup=lightening_menu_keyboard)
        bot.delete_message(CHANNEL_NAME, message.message_id - 1)
    elif Menu == 11:
        bot.delete_message(CHANNEL_NAME, message.message_id, timeout = 0.5)
        therm_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)      
        if message.text == 'Вимкнути котел 🔥' or message.text == 'Запустити котел 🔥':    
            uart('tel_Mt_4')
            sleep(2)
        if message.text == 'Вимкнути "еко-режим" 🔥'or message.text == 'Активувати "еко-режим" 🔥':    
            uart('tel_Mt_5')
            sleep(2)                      
        if 'котел' in message.text or 'режим' in message.text or 'дані' in message.text:
            SQL_request_therm_boiler_data()
            if My_string.find('Базовий термостат 🔥: on')>-1:
                therm_button_1 = telebot.types.KeyboardButton(text='Вимкнути котел 🔥')
            if My_string.find('Базовий термостат 🔥: off')>-1:
                therm_button_1 = telebot.types.KeyboardButton(text='Запустити котел 🔥')
            therm_button_2 = telebot.types.KeyboardButton(text='Базова уставка °С')    
            if My_string.find('Еко-термостат (охорона) 🔥: on')>-1:
                therm_button_3 = telebot.types.KeyboardButton(text='Вимкнути "еко-режим" 🔥')
            if My_string.find('Еко-термостат (охорона) 🔥: off')>-1:
                therm_button_3 = telebot.types.KeyboardButton(text='Активувати "еко-режим" 🔥')
            therm_button_4 = telebot.types.KeyboardButton(text='Еко-уставка °С')        
            refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
            back_btn = telebot.types.KeyboardButton(text='Назад')
            main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
            therm_menu_keyboard.add(therm_button_1, therm_button_2)
            therm_menu_keyboard.add(therm_button_3, therm_button_4)
            therm_menu_keyboard.add(back_btn, refresh_therm_btn)
            therm_menu_keyboard.add(main_menu_btn)
        if message.text == 'Базова уставка °С': 
            Menu = 111
            My_string = '1.1.1\nВведіть нове значення очікуваної температури повітря у форматі: [xx.x]\nMin: 12.0\nMax: 26.0'
        if message.text == 'Еко-уставка °С': 
            Menu = 112
            My_string = '1.1.2\nВведіть нове значення очікуваної температури повітря у форматі: [xx.x]\nMin: 12.0\nMax: 26.0'
        if 'уставка' in message.text:
            back_btn = telebot.types.KeyboardButton(text='Назад')
            main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
            therm_menu_keyboard.add(back_btn)
            therm_menu_keyboard.add(main_menu_btn)   
        if message.text == 'Назад':    
            Menu = 1        
            SQL_request_therm_data()
            therm_button_1 = telebot.types.KeyboardButton(text='Газовий котел')
            therm_button_2 = telebot.types.KeyboardButton(text='Кондиціонер')
            main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
            refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
            therm_menu_keyboard.add(therm_button_1, therm_button_2)
            therm_menu_keyboard.add(main_menu_btn, refresh_therm_btn)   
        bot.send_message(CHANNEL_NAME, My_string, reply_markup=therm_menu_keyboard)
        bot.delete_message(CHANNEL_NAME, message.message_id - 1)
    elif Menu == 111 or Menu == 112:
        bot.delete_message(CHANNEL_NAME, message.message_id, timeout = 0.5)
        therm_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
        if re.search(r'[0-9.]', message.text) != None:    
            content = message.text
            if len(content) == 4:
                Value = float(content)
                if 12.0 <= Value <= 26.0:
                    if Menu == 111:
                        Str = 'tel_therm_1_' + str(Value)
                    if Menu == 112:
                        Str = 'tel_therm_6_' + str(Value)                 
                    uart(Str)
                    sleep(2)
                    SQL_request_therm_boiler_data()
                    Menu = 11
                    if My_string.find('Базовий термостат 🔥: on')>-1:
                        therm_button_1 = telebot.types.KeyboardButton(text='Вимкнути котел 🔥')
                    if My_string.find('Базовий термостат 🔥: off')>-1:
                        therm_button_1 = telebot.types.KeyboardButton(text='Запустити котел 🔥')
                    therm_button_2 = telebot.types.KeyboardButton(text='Базова уставка °С')    
                    if My_string.find('Еко-термостат (охорона) 🔥: on')>-1:
                        therm_button_3 = telebot.types.KeyboardButton(text='Вимкнути "еко-режим" 🔥')
                    if My_string.find('Еко-термостат (охорона) 🔥: off')>-1:
                        therm_button_3 = telebot.types.KeyboardButton(text='Активувати "еко-режим" 🔥')
                    therm_button_4 = telebot.types.KeyboardButton(text='Еко-уставка °С')        
                    refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
                    back_btn = telebot.types.KeyboardButton(text='Назад')
                    main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
                    therm_menu_keyboard.add(therm_button_1, therm_button_2)
                    therm_menu_keyboard.add(therm_button_3, therm_button_4)
                    therm_menu_keyboard.add(back_btn, refresh_therm_btn)
                    therm_menu_keyboard.add(main_menu_btn)
                else: 
                    My_string = '1.1.1\nВведене значення температури повітря не відповідає робочому діапазону:\nMin: 12.0\nMax: 26.0\nСпробуйте ще раз:'
                    back_btn = telebot.types.KeyboardButton(text='Назад')
                    main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
                    therm_menu_keyboard.add(back_btn)
                    therm_menu_keyboard.add(main_menu_btn)
            else: 
                My_string = '1.1.2\nНекорректне значення температури повітря для термостату!\nФормат: [xx.x]\nСпробуйте ще раз:'
                back_btn = telebot.types.KeyboardButton(text='Назад')
                main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
                therm_menu_keyboard.add(back_btn)
                therm_menu_keyboard.add(main_menu_btn)
            bot.send_message(CHANNEL_NAME, My_string, reply_markup=therm_menu_keyboard)
            bot.delete_message(CHANNEL_NAME, message.message_id - 1)     
    elif Menu == 12:
        bot.delete_message(CHANNEL_NAME, message.message_id, timeout = 0.5)
        therm_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)      
        if message.text == 'Вимкнути кондиціонер 🥶' or message.text == 'Запустити кондиціонер 🥶':    
            uart('tel_Mt_6')
            sleep(2)
        if message.text == 'Вимкнути "еко-режим" 🥶'or message.text == 'Активувати "еко-режим" 🥶':    
            uart('tel_Mt_7')
            sleep(2)                      
        if 'кондиціонер' in message.text or 'режим' in message.text or 'дані' in message.text:
            SQL_request_therm_AC_data()
            if My_string.find('Базовий термостат 🥶: on')>-1:
                therm_button_1 = telebot.types.KeyboardButton(text='Вимкнути кондиціонер 🥶')
            if My_string.find('Базовий термостат 🥶: off')>-1:
                therm_button_1 = telebot.types.KeyboardButton(text='Запустити кондиціонер 🥶')
            therm_button_2 = telebot.types.KeyboardButton(text='Базова уставка °С')    
            if My_string.find('Еко-термостат (охорона) 🥶: on')>-1:
                therm_button_3 = telebot.types.KeyboardButton(text='Вимкнути "еко-режим" 🥶')
            if My_string.find('Еко-термостат (охорона) 🥶: off')>-1:
                therm_button_3 = telebot.types.KeyboardButton(text='Активувати "еко-режим" 🥶')
            therm_button_4 = telebot.types.KeyboardButton(text='Еко-уставка °С')        
            refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
            back_btn = telebot.types.KeyboardButton(text='Назад')
            main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
            therm_menu_keyboard.add(therm_button_1, therm_button_2)
            therm_menu_keyboard.add(therm_button_3, therm_button_4)
            therm_menu_keyboard.add(back_btn, refresh_therm_btn)
            therm_menu_keyboard.add(main_menu_btn)
        if message.text == 'Базова уставка °С': 
            Menu = 121
            My_string = '1.2.1\nВведіть нове значення очікуваної температури повітря у форматі: [xx.x]\nMin: 16.0\nMax: 35.0'
        if message.text == 'Еко-уставка °С': 
            Menu = 122
            My_string = '1.2.2\nВведіть нове значення очікуваної температури повітря у форматі: [xx.x]\nMin: 16.0\nMax: 35.0'
        if 'уставка' in message.text:
            back_btn = telebot.types.KeyboardButton(text='Назад')
            main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
            therm_menu_keyboard.add(back_btn)
            therm_menu_keyboard.add(main_menu_btn)   
        if message.text == 'Назад':    
            Menu = 1        
            SQL_request_therm_data()
            therm_button_1 = telebot.types.KeyboardButton(text='Газовий котел')
            therm_button_2 = telebot.types.KeyboardButton(text='Кондиціонер')
            main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
            refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
            therm_menu_keyboard.add(therm_button_1, therm_button_2)
            therm_menu_keyboard.add(main_menu_btn, refresh_therm_btn)   
        bot.send_message(CHANNEL_NAME, My_string, reply_markup=therm_menu_keyboard)
        bot.delete_message(CHANNEL_NAME, message.message_id - 1)
    elif Menu == 121 or Menu == 122:
        bot.delete_message(CHANNEL_NAME, message.message_id, timeout = 0.5)
        therm_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
        if re.search(r'[0-9.]', message.text) != None:    
            content = message.text
            if len(content) == 4:
                Value = float(content)
                if 16.0 <= Value <= 35.0:
                    if Menu == 121:
                        Str = 'tel_therm_2_' + str(Value)
                    if Menu == 122:
                        Str = 'tel_therm_7_' + str(Value)                 
                    uart(Str)
                    sleep(2)
                    SQL_request_therm_AC_data()
                    Menu = 11
                    if My_string.find('Базовий термостат 🥶: on')>-1:
                        therm_button_5 = telebot.types.KeyboardButton(text='Вимкнути кондиціонер 🥶')
                    if My_string.find('Базовий термостат 🥶: off')>-1:
                        therm_button_5 = telebot.types.KeyboardButton(text='Запустити кондиціонер 🥶')
                    therm_button_6 = telebot.types.KeyboardButton(text='Базова уставка °С')
                    if My_string.find('Еко-термостат (охорона) 🥶: on')>-1:
                        therm_button_7 = telebot.types.KeyboardButton(text='Вимкнути "еко-режим" 🥶')
                    if My_string.find('Еко-термостат (охорона) 🥶: off')>-1:
                        therm_button_7 = telebot.types.KeyboardButton(text='Активувати "еко-режим" 🥶')
                    therm_button_8 = telebot.types.KeyboardButton(text='Еко-уставка °С')        
                    refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
                    back_btn = telebot.types.KeyboardButton(text='Назад')
                    main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
                    therm_menu_keyboard.add(therm_button_5, therm_button_6)
                    therm_menu_keyboard.add(therm_button_7, therm_button_8)
                    therm_menu_keyboard.add(back_btn, refresh_therm_btn)
                    therm_menu_keyboard.add(main_menu_btn)
                else: 
                    My_string = '1.2.1 Введене значення температури повітря не відповідає робочому діапазону:\nMin: 16.0\nMax: 35.0\nСпробуйте ще раз:'
                    back_btn = telebot.types.KeyboardButton(text='Назад')
                    main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
                    therm_menu_keyboard.add(back_btn)
                    therm_menu_keyboard.add(main_menu_btn)
            else: 
                My_string = '1.2.2 Некорректне значення температури повітря для термостату!\nФормат: [xx.x]\nСпробуйте ще раз:'
                back_btn = telebot.types.KeyboardButton(text='Назад')
                main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
                therm_menu_keyboard.add(back_btn)
                therm_menu_keyboard.add(main_menu_btn)
            bot.send_message(CHANNEL_NAME, My_string, reply_markup=therm_menu_keyboard)
            bot.delete_message(CHANNEL_NAME, message.message_id - 1)   

    elif Menu == 3:
        bot.delete_message(CHANNEL_NAME, message.message_id, timeout = 0.5)
        guard_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
        if 'охорону' in message.text:    
            uart('Security_State')
        if 'тривогу' in message.text:
            uart('Alarm_State')
        sleep(2)
        SQL_request_guard_data()
        if My_string.find('Охорона: on')>-1:
            guard_button_1 = telebot.types.KeyboardButton(text='Вимкнути охорону')
        if My_string.find('Охорона: off')>-1:
            guard_button_1 = telebot.types.KeyboardButton(text='Активувати охорону')
        if My_string.find('Тривога: on 🔔')>-1:
            guard_button_2 = telebot.types.KeyboardButton(text='Вимкнути тривогу')
        if My_string.find('Тривога: off 🔕')>-1:
            guard_button_2 = telebot.types.KeyboardButton(text='Активувати тривогу')
        main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
        refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
        guard_menu_keyboard.add(guard_button_1, guard_button_2)
        guard_menu_keyboard.add(main_menu_btn, refresh_therm_btn)      
        bot.send_message(CHANNEL_NAME, My_string, reply_markup=guard_menu_keyboard)
        bot.delete_message(CHANNEL_NAME, message.message_id - 1)
    elif Menu == 4:
        bot.delete_message(CHANNEL_NAME, message.message_id, timeout = 1)
        lightening_menu_keyboard = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
        if 'Передпокій' in message.text:
            uart('tel_bt_1')
        if 'Спальня' in message.text:
            uart('tel_bt_2')
        if 'Вбиральня' in message.text:
            uart('tel_bt_3')
        if 'Сан. вузол' in message.text:
            uart('tel_bt_4')
        if 'Вітальня' in message.text:
            uart('tel_bt_5')
        if 'Кухня' in message.text:
            uart('tel_bt_6')
        if 'Кабінет' in message.text:
            uart('tel_bt_7')
        if 'Витяжка у сан-вузлу' in message.text:
            uart('tel_bt_8')
        if 'Витяжка у спальні' in message.text:
            uart('tel_bt_9')
        if 'Amplifier' in message.text:
            uart('tel_bt_10')
            sleep(2)
        if 'Bluetooth' in message.text:
            uart('tel_bt_11')
            sleep(2)
        if "Kill'em All!" in message.text:
            uart('tel_bt_12')
        sleep(1)
        SQL_request_lightening_data()
        btn_1 = types.InlineKeyboardButton(text="Kill'em All!")
        if My_string.find('Передпокій: off')>-1:
            btn_2 = telebot.types.KeyboardButton(text='Передпокій')
        if My_string.find('Передпокій: on')>-1:
            btn_2 = telebot.types.KeyboardButton(text='Передпокій💡')
        if My_string.find('Спальня: off')>-1:
            btn_3 = telebot.types.KeyboardButton(text='Спальня')
        if My_string.find('Спальня: on')>-1:
            btn_3 = telebot.types.KeyboardButton(text='Спальня💡')        
        if My_string.find('Вбиральня: off')>-1:
            btn_4 = telebot.types.KeyboardButton(text='Вбиральня')
        if My_string.find('Вбиральня: on')>-1:
            btn_4 = telebot.types.KeyboardButton(text='Вбиральня💡')        
        if My_string.find('Сан. вузол: off')>-1:
            btn_5 = telebot.types.KeyboardButton(text='Сан. вузол')
        if My_string.find('Сан. вузол: on')>-1:
            btn_5 = telebot.types.KeyboardButton(text='Сан. вузол💡')
        if My_string.find('Вітальня: off')>-1:
            btn_6 = telebot.types.KeyboardButton(text='Вітальня')
        if My_string.find('Вітальня: on')>-1:
            btn_6 = telebot.types.KeyboardButton(text='Вітальня💡')
        if My_string.find('Кухня: off')>-1:
            btn_7 = telebot.types.KeyboardButton(text='Кухня')
        if My_string.find('Кухня: on')>-1:
            btn_7 = telebot.types.KeyboardButton(text='Кухня💡')
        if My_string.find('Кабінет: off')>-1:
            btn_8 = telebot.types.KeyboardButton(text='Кабінет')
        if My_string.find('Кабінет: on')>-1:
            btn_8 = telebot.types.KeyboardButton(text='Кабінет💡')
        if My_string.find('Витяжка у сан-вузлу: off')>-1:
            btn_9 = telebot.types.KeyboardButton(text='Витяжка у сан-вузлу')
        if My_string.find('Витяжка у сан-вузлу: on')>-1:
            btn_9 = telebot.types.KeyboardButton(text='Витяжка у сан-вузлу 💨')
        if My_string.find('Витяжка у спальні: off')>-1:
            btn_10 = telebot.types.KeyboardButton(text='Витяжка у спальні')
        if My_string.find('Витяжка у спальні: on')>-1:
            btn_10 = telebot.types.KeyboardButton(text='Витяжка у спальні 💨')
        if My_string.find('Amplifier: off')>-1:
            btn_11 = telebot.types.KeyboardButton(text='Amplifier')
        if My_string.find('Amplifier: on')>-1:
            btn_11 = telebot.types.KeyboardButton(text='Amplifier 🔊')
        if My_string.find('Bluetooth: off')>-1:
            btn_12 = telebot.types.KeyboardButton(text='Bluetooth')
        if My_string.find('Bluetooth: on')>-1:
            btn_12 = telebot.types.KeyboardButton(text='Bluetooth 📡')
        refresh_therm_btn = telebot.types.KeyboardButton(text='Оновити дані')
        main_menu_btn = telebot.types.KeyboardButton(text='Головне меню')
        lightening_menu_keyboard.add(btn_1, btn_2)
        lightening_menu_keyboard.add(btn_3, btn_4)
        lightening_menu_keyboard.add(btn_5, btn_6)
        lightening_menu_keyboard.add(btn_7, btn_8)
        lightening_menu_keyboard.add(btn_9, btn_10)
        lightening_menu_keyboard.add(btn_11, btn_12)
        lightening_menu_keyboard.add(main_menu_btn, refresh_therm_btn) 
        bot.send_message(CHANNEL_NAME, My_string, reply_markup=lightening_menu_keyboard)
        bot.delete_message(CHANNEL_NAME, message.message_id - 1)

def uart(msg):
    ser = serial.Serial('/dev/ttyS5', 57600, timeout=1)
    ser.write(msg.encode('utf-8'))
    time.sleep(0.5)

def SQL_request_therm_data():
    myconn = mysql.connector.connect(host = "localhost", user = "root",passwd = "11223344root",database = "s_h") 
    cur = myconn.cursor() 
    global My_string
    My_string = '1. МІКРО-КЛІМАТ:\n\n'
    try: #Reading the Employee data         
        cur.execute("select name, value, unit from p_m where id in(6,7,8,9)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + ' ' + str(i[2]) + '\n'
    except: 
        myconn.rollback()
    try: #Reading the Employee data         
        cur.execute("select name, state from m_s where id in(4,6)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += '\n' + str(i[0]) + ': ' + str(i[1])    
    except: 
        myconn.rollback()   
    try: #Reading the Employee data         
        cur.execute("select name, state from a_s where id in(11)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += '\n\n' + str(i[0]) + ': ' + str(i[1])
    except: 
       myconn.rollback() 
    My_string = My_string.replace('Термостат до котла: on', '🔥 Котел/термостат: в роботі')
    My_string = My_string.replace('Термостат до котла: off', '🔥 Котел/термостат: вимкнено')
    My_string = My_string.replace('Термостат до кондиціонера (на охолодження): on', '🥶 Конд-р/термостат: в роботі')
    My_string = My_string.replace('Термостат до кондиціонера (на охолодження): off', '🥶 Конд-р/термостат: вимкнено') 
    My_string = My_string.replace('Вікно у вітальні: on', '🪟 Вікно у вітальні: відчинено')
    My_string = My_string.replace('Вікно у вітальні: off', '🪟 Вікно у вітальні: зачинено')   

def SQL_request_therm_boiler_data():
    myconn = mysql.connector.connect(host = "localhost", user = "root",passwd = "11223344root",database = "s_h") 
    cur = myconn.cursor() 
    global My_string
    My_string = '1.1 Налаштування термостату котла:\n\n'
    try: #Reading the Employee data         
        cur.execute("select name, state from m_s where id in(4)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + '\n'
    except: 
        myconn.rollback()
    try: #Reading the Employee data         
        cur.execute("select name, value, unit from p_m where id in(10)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + ' ' + str(i[2]) + '\n\n'
    except: 
        myconn.rollback()
    try: #Reading the Employee data         
        cur.execute("select name, state from m_s where id in(5)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + '\n'
    except: 
        myconn.rollback()
    try: #Reading the Employee data         
        cur.execute("select name, value, unit from p_m where id in(15)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + ' ' + str(i[2]) + '\n\n'
    except: 
        myconn.rollback()
    try: #Reading the Employee data         
        cur.execute("select name, value, unit from p_m where id in(17,12)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + ' ' + str(i[2]) + '\n'
    except: 
        myconn.rollback()
    My_string += '\n*(щоб вимкнути котел,\nтреба спочатку вимкнути\n"еко-режим" 🔥)'
    My_string = My_string.replace('Термостат до котла', 'Базовий термостат 🔥')
    My_string = My_string.replace('Економ-обігрів у режимі охорони', 'Еко-термостат (охорона) 🔥')
    My_string = My_string.replace('Уставка до котла (Therm_1)', 'Базова уставка °С')
    My_string = My_string.replace('Min уставка еко-режиму до котла (Therm_6)', 'Еко-уставка °С')
    My_string = My_string.replace('Гістерезис котла', 'Зона коливань')

def SQL_request_therm_AC_data():
    myconn = mysql.connector.connect(host = "localhost", user = "root",passwd = "11223344root",database = "s_h") 
    cur = myconn.cursor() 
    global My_string
    My_string = '1.2 Налаштування термостату кондиціонера:\n\n'
    try: #Reading the Employee data         
        cur.execute("select name, state from m_s where id in(6)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + '\n'
    except: 
        myconn.rollback()
    try: #Reading the Employee data         
        cur.execute("select name, value, unit from p_m where id in(11)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + ' ' + str(i[2]) + '\n\n'
    except: 
        myconn.rollback()
    try: #Reading the Employee data         
        cur.execute("select name, state from m_s where id in(7)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + '\n'
    except: 
        myconn.rollback()
    try: #Reading the Employee data         
        cur.execute("select name, value, unit from p_m where id in(16)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + ' ' + str(i[2]) + '\n\n'
    except: 
        myconn.rollback()
    try: #Reading the Employee data         
        cur.execute("select nsystemctlame, value, unit from p_m where id in(20)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + ' ' + str(i[2]) + '\n'
    except: 
        myconn.rollback()
    My_string += '*(щоб вимкнути кондиціонер,\nтреба спочатку вимкнути\n"еко-режим" 🥶)'
    My_string = My_string.replace('Швидкість обдуву', 'Швидкість обдуву💨')
    My_string = My_string.replace('Термостат до кондиціонера (на охолодження)', 'Базовий термостат 🥶')
    My_string = My_string.replace('Економ-охолодження у режимі охорони', 'Еко-термостат (охорона) 🥶')
    My_string = My_string.replace('Уставка до кондиц. (Therm_2)', 'Базова уставка')
    My_string = My_string.replace('Max уставка еко-режиму до кондиц (Therm_7)', 'Еко-уставка')
    My_string = My_string.replace('Поточна швидкість обдуву кондиціонера', 'Швидкість обдуву')

def SQL_request_power_data():
    myconn = mysql.connector.connect(host = "localhost", user = "root",passwd = "11223344root",database = "s_h") 
    cur = myconn.cursor() 
    global My_string
    SQL_p_m = 0
    SQL_m_s = 0
    try: #Reading the Employee data         
        cur.execute("select name, value, unit from p_m where id in(1,2,3,4,5)") 
        SQL_p_m = cur.fetchall() 
    except: 
       myconn.rollback() 
    try: #Reading the Employee data         
        cur.execute("select name, state from m_s where id in(1,2,3)") 
        SQL_m_s = cur.fetchall()    
    except: 
       myconn.rollback()
    myconn.close()
    My_string = '2. ЕНЕРГО-АУДИТ:\n'
    My_string = '\n--Мережа--\n'
    My_string += SQL_p_m[0][0] + ': ' + SQL_p_m[0][1] + ' ' + SQL_p_m[0][2] + '\n'
    My_string += SQL_p_m[1][0] + ': ' + SQL_p_m[1][1] + ' ' + SQL_p_m[1][2] + '\n'
    My_string += SQL_p_m[2][0] + ': ' + SQL_p_m[2][1] + ' ' + SQL_p_m[2][2] + '\n'
    My_string += SQL_p_m[3][0] + ': ' + SQL_p_m[3][1] + ' ' + SQL_p_m[3][2] + '\n'
    My_string += SQL_p_m[4][0] + ': ' + SQL_p_m[4][1] + ' ' + SQL_p_m[4][2] + '\n'
    My_string += '\n--ДБЖ--\n'
    My_string += '• ' + SQL_m_s[0][0] + ': ' + SQL_m_s[0][1] + '\n'
    My_string += '• ' + SQL_m_s[1][0] + ': ' + SQL_m_s[1][1] + '\n'
    My_string += '• ' + SQL_m_s[2][0] + ': ' + SQL_m_s[2][1] + '\n'
    My_string = My_string.replace('on', 'ON')
    My_string = My_string.replace('off', 'OFF')

def SQL_request_guard_data():
    myconn = mysql.connector.connect(host = "localhost", user = "root",passwd = "11223344root",database = "s_h") 
    cur = myconn.cursor()
    global My_string
    SQL_result = 0
    My_string = '3. СТАН ОХОРОНИ ТА НАЛАШТУВАННЯ:\n\n'     
    try: #Reading the Employee data         
        cur.execute("select name, state from a_s where id in(1,2,3,4,5,6,7,8,9,10,11)") 
        SQL_result = cur.fetchall() 
    except: 
       myconn.rollback() 
    myconn.close()
    My_string += SQL_result[0][0] + ': ' + SQL_result[0][1] + '\n'
    My_string += SQL_result[1][0] + ': ' + SQL_result[1][1] + '\n\n'
    My_string += SQL_result[2][0] + ': ' + SQL_result[2][1] + '\n'
    My_string += SQL_result[3][0] + ': ' + SQL_result[3][1] + '\n'
    My_string += SQL_result[10][0] + ': ' + SQL_result[10][1] + '\n\n'
    My_string += SQL_result[7][0] + ': ' + SQL_result[7][1] + '\n'
    My_string += SQL_result[4][0] + ': ' + SQL_result[4][1] + '\n'
    My_string += SQL_result[5][0] + ': ' + SQL_result[5][1] + '\n'
    My_string += SQL_result[6][0] + ': ' + SQL_result[6][1] + '\n'
    My_string += SQL_result[9][0] + ': ' + SQL_result[9][1] + '\n'
    My_string += SQL_result[8][0] + ': ' + SQL_result[8][1]
    My_string = My_string.replace('Тривога: on', 'Тривога: on 🔔')
    My_string = My_string.replace('Тривога: off', 'Тривога: off 🔕')   
    My_string = My_string.replace('Охорона: on', 'Охорона: on 👮‍♂️')
    My_string = My_string.replace('Охорона: off', 'Охорона: off 😴')
    My_string = My_string.replace('Вхідний замок: on', 'Вхідний замок: зачинено')
    My_string = My_string.replace('Вхідний замок: off', 'Вхідний замок: відчинено')
    My_string = My_string.replace('Вхідні двері: on', 'Вхідні двері: зачинено')
    My_string = My_string.replace('Вхідні двері: off', 'Вхідні двері: відчинено')
    My_string = My_string.replace('Вікно у вітальні: on', 'Вікно у вітальні: відчинено')
    My_string = My_string.replace('Вікно у вітальні: off', 'Вікно у вітальні: зачинено')

def SQL_request_lightening_data():
    myconn = mysql.connector.connect(host = "localhost", user = "root",passwd = "11223344root",database = "s_h") 
    cur = myconn.cursor() 
    global My_string
    My_string = '4. СТАН ОСВІТЛЕННЯ:\n\n'    
    try: #Reading the Employee data         
        cur.execute("select name, state from lls where id in(1,2,3,4,5,6,7,8,9,10,11)") 
        SQL_result = cur.fetchall() 
        for i in SQL_result: 
            My_string += str(i[0]) + ': ' + str(i[1]) + '\n'
    except: 
       myconn.rollback() 
    myconn.close()
    My_string = My_string.replace('on', 'on💡')
    My_string = My_string.replace('Витяжка (вбир./спальня)', 'Витяжка у спальні')
    My_string = My_string.replace('Витяжка (сан. вузол)', 'Витяжка у сан-вузлу')

while True: 
    try: bot.polling(none_stop=True) 
    except Exception as e: 
        print(e) 
        time.sleep(10)
