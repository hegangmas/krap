# -*- coding: utf-8 -*-
#under opend licences like eupl or gpl3 havily based on tutorial from youtube
#becouse i prefer to teleport my snake
import pygame
import time
import random

pygame.init()
direction = 3
white = (255,255,255)
black = (0,0,0)
red = (200,0,0)
green = (0,255,0)
blue = (0,0,255)
blocksize = 10
AppleThickness = 20
FPS = 15
display_width = 800
display_height  = 600


gameDisplay = pygame.display.set_mode((display_width,display_height))
pygame.display.set_caption('Kačon')
icon = pygame.image.load('jabuk.png')
pygame.display.set_icon(icon)
clock = pygame.time.Clock()
img = pygame.image.load('kacon.png')
apl=pygame.image.load('jabuk.png')
smallfont = pygame.font.SysFont(None, 25)
medfont = pygame.font.SysFont(None, 50)
largefont = pygame.font.SysFont(None, 75)

def pause():
    paused=True
    while paused:
        for event in pygame.event.get():
            if event.type==pygame.QUIT:
                pygame.quit()
                quit()
            if event.type==pygame.KEYDOWN:
                if event.key == pygame.K_c:
                    paused =False
                elif event.key == pygame.K_q:
                    pygame.quit()
                    quit()
        gameDisplay.fill(black)
        message_to_screen("Paused",white,ydisplace=-50,size="large")
        message_to_screen("press C to play or Q to quit",white,ydisplace=80,size="small")
        pygame.display.update()
        clock.tick(4)
    

def randAppleGen():
    randAppleX = round(random.randrange(0, display_width-AppleThickness)/10.0)*10.0
    randAppleY = round(random.randrange(0, display_height-AppleThickness)/10.0)*10.0
    return randAppleX,randAppleY

def score(score):
    text = smallfont.render(str(score),True, white)
    gameDisplay.blit(text,[0,0])
def gameintro():
    intro = True
    while intro:
        for event in pygame.event.get():
            if event.type==pygame.QUIT:
                pygame.quit()
                quit()
            if event.type==pygame.KEYDOWN:
                if event.key == pygame.K_c:
                    intro =False
            if event.type==pygame.KEYDOWN:
                if event.key == pygame.K_q:
                    pygame.quit()
                    quit()
                    
        gameDisplay.fill(black)
        message_to_screen("Kacon :)",green,ydisplace=-50,size="large")
        message_to_screen("Eat red apples",white,ydisplace=20,size="small")
        message_to_screen("&do not eat yourself",red,ydisplace=60,size="small")
        message_to_screen("press C to play, P pause or Q to quit",white,ydisplace=80,size="small")

        pygame.display.update()
        clock.tick(4)

def snake(blocksize, snakelist, direction):
    if direction == 2:#down
        head = pygame.transform.rotate(img,180)
    if direction == 1:#up
        head = img
    if direction == 4:#left
        head = pygame.transform.rotate(img,270)
    if direction == 3:#right
        head = pygame.transform.rotate(img,90)        
    gameDisplay.blit(head,[snakelist[-1][0],snakelist[-1][1]])
    for XY in snakelist[:-1]:
        pygame.draw.rect(gameDisplay, green, [XY[0],XY[1],blocksize,blocksize])

def text_objects(text,color,size):
    if size=="small":
        textSurface = smallfont.render(text, True, color)
    elif size=="medium":
        textSurface = medfont.render(text, True, color)
    elif size=="large":
        textSurface = largefont.render(text, True, color)
    return textSurface, textSurface.get_rect()
    
def message_to_screen(msg,color,ydisplace=0,size="small"):
    textSurf, textRect = text_objects(msg,color,size)
    textRect.center = (display_width / 2), (display_height / 2)+ydisplace
    gameDisplay.blit(textSurf, textRect)

def gameLoop():
    global direction
    direction=3
    gameExit = False
    gameOver = False
    x = display_width/2
    y = display_height/2
    x_d = 0
    y_d = 0
    snakeList = []
    snakeLength = 1
    randAppleX, randAppleY=randAppleGen()
    
    while not gameExit:
        while gameOver == True:
            gameDisplay.fill(black)
            message_to_screen("Game over :(",red,ydisplace=-50,size="large")
            message_to_screen("press C to play again or Q to quit", white,10,size="medium")
            pygame.display.update()
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    gameOver = False
                    gameExit = True
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_q:
                        gameExit = True
                        gameOver = False
                    if event.key == pygame.K_c:
                        gameLoop()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                gameExit = True
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT and direction!=4:
                    direction =3
                elif event.key == pygame.K_RIGHT and direction!=3:
                    direction =4
                elif event.key == pygame.K_UP and direction!=2:
                    direction =1
                elif event.key == pygame.K_DOWN and direction!=1:
                    direction =2
                elif event.key == pygame.K_p:
                    pause()
        if direction == 1:
            y-= blocksize
        if direction == 2:
            y+= blocksize
        if direction == 3:
            x -= blocksize
        if direction == 4:
            x += blocksize
        if x > display_width-blocksize:
            x=0
        if y > display_height-blocksize:
            y=0
        if x < 0:
            x=display_width-blocksize
        if y < 0:
            y=display_height-blocksize
        gameDisplay.fill(black)
 
        #pygame.draw.rect(gameDisplay, red, [randAppleX, randAppleY, AppleThickness, AppleThickness])
        gameDisplay.blit(apl,(randAppleX, randAppleY) )

        snakeHead = []
        snakeHead.append(x)
        snakeHead.append(y)
        snakeList.append(snakeHead)
        if len(snakeList) > snakeLength:
            del snakeList[0]
        for eachSegment in snakeList[:-1]:
            if eachSegment == snakeHead:
                gameOver = True
        snake(blocksize, snakeList,direction)
        score(snakeLength-1)
        pygame.display.update()
        if x > randAppleX and x < randAppleX + AppleThickness or x + blocksize > randAppleX and x + blocksize < randAppleX + AppleThickness:
            if y > randAppleY and y < randAppleY + AppleThickness:
                randAppleX, randAppleY=randAppleGen()
                snakeLength += 1
            elif y + blocksize > randAppleY and y + blocksize < randAppleY + AppleThickness:
                randAppleX, randAppleY=randAppleGen()
                snakeLength += 1

        clock.tick(FPS)
        
    pygame.quit()
    quit()
gameintro()
gameLoop()
