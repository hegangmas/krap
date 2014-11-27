# -*- coding: utf-8 -*-
#under opend licences havily based on tutorial from youtube
import pygame
import time
import random

pygame.init()

white = (255,255,255)
black = (0,0,0)
red = (255,0,0)
green = (0,155,0)

display_width = 400
display_height  = 300

gameDisplay = pygame.display.set_mode((display_width,display_height))
pygame.display.set_caption('Kačon')



clock = pygame.time.Clock()

block_size = 10
FPS = 30

font = pygame.font.SysFont(None, 25)

def snake(block_size, snakelist):
    for XnY in snakelist:
        pygame.draw.rect(gameDisplay, green, [XnY[0],XnY[1],block_size,block_size])

def text_objects(text,color):
    textSurface = font.render(text, True, color)
    return textSurface, textSurface.get_rect()
    
    
def message_to_screen(msg,color):
    textSurf, textRect = text_objects(msg,color)
    textRect.center = (display_width / 2), (display_height / 2)
    gameDisplay.blit(textSurf, textRect)


def gameLoop():
    gameExit = False
    gameOver = False

    lead_x = display_width/2
    lead_y = display_height/2

    lead_x_change = 0
    lead_y_change = 0

    snakeList = []
    snakeLength = 1
    direction =3

    randAppleX = round(random.randrange(0, display_width-block_size))#/10.0)*10.0
    randAppleY = round(random.randrange(0, display_height-block_size))#/10.0)*10.0
    
    while not gameExit:

        while gameOver == True:
            gameDisplay.fill(white)
            message_to_screen(":(, press C to play again or Q to quit", red)
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



        if direction ==1:
            lead_y-= block_size
        if direction ==2:
            lead_y+= block_size
        if direction == 3:
            lead_x -= block_size
        if direction == 4:
            lead_x += block_size

            
        if lead_x > display_width:
            lead_x=0
        if lead_y > display_height:
            lead_y=0
        if lead_x < 0:
            lead_x=display_width
        if lead_y < 0:
            lead_y=display_height
            
        gameDisplay.fill(white)

        AppleThickness = 20
        pygame.draw.rect(gameDisplay, red, [randAppleX, randAppleY, AppleThickness, AppleThickness])

        
        snakeHead = []
        snakeHead.append(lead_x)
        snakeHead.append(lead_y)
        snakeList.append(snakeHead)

        if len(snakeList) > snakeLength:
            del snakeList[0]

        for eachSegment in snakeList[:-1]:
            if eachSegment == snakeHead:
                gameOver = True

        
        snake(block_size, snakeList)

        
        pygame.display.update()


        if lead_x > randAppleX and lead_x < randAppleX + AppleThickness or lead_x + block_size > randAppleX and lead_x + block_size < randAppleX + AppleThickness:

            if lead_y > randAppleY and lead_y < randAppleY + AppleThickness:

                randAppleX = round(random.randrange(0, display_width-block_size))#/10.0)*10.0
                randAppleY = round(random.randrange(0, display_height-block_size))#/10.0)*10.0
                snakeLength += 1

            elif lead_y + block_size > randAppleY and lead_y + block_size < randAppleY + AppleThickness:

                randAppleX = round(random.randrange(0, display_width-block_size))#/10.0)*10.0
                randAppleY = round(random.randrange(0, display_height-block_size))#/10.0)*10.0
                snakeLength += 1


        clock.tick(FPS)
        
    pygame.quit()
    quit()


gameLoop()
