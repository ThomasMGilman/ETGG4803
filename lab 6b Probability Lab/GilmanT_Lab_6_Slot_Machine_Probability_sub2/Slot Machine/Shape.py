import pygame
from enum import Enum


class ShapeType(Enum):
    RECTANGLE = 1
    CIRCLE = 2


class Shape:
    def __init__(self, pos, shape_type):
        self.position = pos
        self.shape_type = shape_type


class Rectangle(Shape):
    def __init__(self, position, half_width, half_height, is_button=False):
        super().__init__(position, ShapeType.RECTANGLE)
        self.is_button = is_button
        self.update_dimensions(half_width, half_height)

    def update_dimensions(self, new_half_width, new_half_height):
        """Update the dimensions of the Rectangle with new half_Width and half_Height"""
        self.half_width = new_half_width
        self.half_height = new_half_height
        self.width = new_half_width * 2
        self.height = new_half_height * 2
        self.top_left = (self.position[0] - self.half_width, self.position[1] - self.half_height)
        self.bottom_right = (self.position[0] + self.half_width, self.position[1] + self.half_height)
        self.Rect = pygame.Rect(self.top_left, self.bottom_right)

    def move(self, x, y):
        """Moves the Rectangles Position by the x and y amount given"""
        self.Rect.move(x, y)

    def draw_rectangle_with_image(self, screen, image):
        screen.blit(image, self.Rect)

    def draw_rectangle_with_text(self, screen, text):
        screen.blit(text, self.Rect)

    def check_in_bounds(self, x, y):
        """Check if x, y point given is within bounds"""
        if self.position[0] - self.half_width < x < self.position[0] + self.half_width and \
                self.position[1] - self.half_height < y < self.position[1] + self.half_height:
            return True
        return False
