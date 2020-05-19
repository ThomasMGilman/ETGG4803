import pygame
from Shape import Rectangle


class Text:
    def __init__(self, position, text, font_name, font_size, colour=(0, 0, 0)):
        self.position = position
        self.colour = colour
        self.font_name = font_name
        self.font_size = font_size
        self.font = pygame.font.Font(font_name, font_size)
        self.text = text
        self.text_surface = self.font.render(self.text, True, self.colour)
        font_dimensions = self.font.size(self.text)
        self.Rect = Rectangle(self.position, font_dimensions[0] // 2, font_dimensions[1] // 2)

    def update_font_color(self, new_colour):
        self.colour = new_colour
        self.update_text()

    def update_font(self, font_name, font_size):
        self.font_name = font_name
        self.font_size = font_size
        self.font = pygame.font.Font(font_name, font_size)
        self.update_text()

    def update_text(self, new_text=None):
        if new_text is not None:
            self.text = new_text
            self.text_surface = self.font.render(self.text, True, self.colour)
            font_dimensions = self.font.size(self.text)
            self.Rect.update_dimensions(font_dimensions[0] // 2, font_dimensions[1] // 2)

    def update_position(self, new_position):
        self.Rect.position = new_position

    def move(self, direction):
        self.Rect.move(direction[0], direction[1])

    def update(self, new_text=None):
        if new_text != self.text:
            self.update_text(new_text)

    def draw(self, screen):
        self.Rect.draw_rectangle_with_text(screen, self.text_surface)
