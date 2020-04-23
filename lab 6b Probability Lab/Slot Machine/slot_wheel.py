import pygame
import random
from Shape import Rectangle
from os import walk, path


class SlotWheel:
    Images = []
    images_set = False

    def __init__(self, position, half_width, half_height, assets_file):
        self.Rect = Rectangle(position, half_width, half_height)
        self.load_images(assets_file)
        self.symbol_index = 0

    def load_images(self, image_dir):
        """Parse all the image files in the slot image assets folder,
        and set them only once for all SlotWheel class objects"""
        if not SlotWheel.images_set:
            for root, dirs, files in walk(image_dir):
                for name in files:
                    if name.endswith(".png" or ".jpg"):
                        file_path = path.join(root, name)
                        image = pygame.transform.scale(pygame.image.load(file_path),
                                                       (self.Rect.width, self.Rect.height))
                        self.Images.append((image, name.strip(".png" or ".jpg")))
            SlotWheel.images_set = True

    def randomly_pick_symbol(self):
        self.symbol_index = random.randrange(0, 4)

    def get_slot_symbol_name(self):
        """Get a string representation of the symbol shown on this slots Rectangle.
            Symbol name is the Image Files name."""
        return self.Images[self.symbol_index][1]

    def update_rect(self, direction):
        """Move this Slots Rectangle position by the direction amount"""
        self.Rect.move(direction.x, direction.y)

    def update(self, screen):
        """Randomly Get a symbol and Blit This Slots Rectangle with that symbol to the Screen"""
        self.Rect.draw_rectangle_with_image(screen, SlotWheel.Images[self.symbol_index][0])
