//
//  ViewController.m
//  VGSKeyboard
//
//  Created by Yoji Suzuki on 2017/01/28.
//  Copyright © 2017年 SUZUKI PLAN. All rights reserved.
//

#import "ViewController.h"
#import "MIKMIDI.h"
#import "VGSView.h"
#import "vgs2.h"

int vgsint_init(const char *bin);

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // self.view.frame = [NSScreen mainScreen].frame;
    CALayer *layer = [CALayer layer];
    [layer setBackgroundColor:CGColorCreateGenericRGB(0.0, 0.0, 0.0, 1.0)];
    [self.view setWantsLayer:YES];
    [self.view setLayer:layer];
    
    // initialize VGS system
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSString *docs_dir = [paths objectAtIndex:0];
    BOOL isDir;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:docs_dir isDirectory:&isDir]) {
        if (![fileManager createDirectoryAtPath:docs_dir withIntermediateDirectories:YES attributes:nil error:NULL]) {
            NSLog(@"Error: Create folder failed: %@", docs_dir);
        }
    }
    vgsint_setdir([docs_dir UTF8String]);
    NSString *fullPath;
    fullPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"romdata.bin"];
    if (vgsint_init([fullPath UTF8String])) {
        _exit(-1);
    }
    
    // initialize VGS view
    self.vram = [[VGSView alloc] initWithFrame:[self calcVramRect]];
    [self.view addSubview:self.vram];
    
    // setup input
    [self.view addTrackingRect:self.view.frame owner:[self.view window] userData:nil assumeInside:NO];
    self.view.window.title = @"VGSKeyboard";
    [[NSNotificationCenter defaultCenter] addObserverForName:NSWindowDidResizeNotification
                                                      object:self.view.window
                                                       queue:[NSOperationQueue mainQueue]
                                                  usingBlock:^(NSNotification *notif) {
                                                      // self.bg.frame = self.view.window.contentView.frame;
                                                      self.vram.frame = [self calcVramRect];
                                                  }];
    [[NSNotificationCenter defaultCenter] addObserverForName:NSViewFrameDidChangeNotification
                                                      object:self.view
                                                       queue:[NSOperationQueue mainQueue]
                                                  usingBlock:^(NSNotification *note) {
                                                      // self.bg.frame = self.view.window.contentView.frame;
                                                      self.vram.frame = [self calcVramRect];
                                                  }];
    
    // init MIDI keyboard
    [self initMidiKeyboard];
}

- (BOOL)initMidiKeyboard {
    BOOL result = NO;
    MIKMIDIDeviceManager* dm = [MIKMIDIDeviceManager sharedDeviceManager];
    NSArray<MIKMIDIDevice*>* devices = [dm availableDevices];
    for (MIKMIDIDevice* device in devices) {
        NSArray<MIKMIDIEntity*>* entities = device.entities;
        for (MIKMIDIEntity* entity in entities) {
            NSArray<MIKMIDISourceEndpoint*>* sources = entity.sources;
            for (MIKMIDISourceEndpoint* source in sources) {
                NSLog(@"source: %@", source);
                [dm connectInput:source error:nil eventHandler:^(MIKMIDISourceEndpoint * _Nonnull source, NSArray<MIKMIDICommand *> * _Nonnull commands) {
                    NSLog(@"detected commands: %@", commands);
                }];
                result = YES;
            }
            NSArray<MIKMIDIDestinationEndpoint*>* destinations = entity.destinations;
            for (MIKMIDIDestinationEndpoint* destination in destinations) {
                NSLog(@"destination: %@", destination);
            }
        }
    }
    return result;
}

- (void)didRespond {
}

- (NSRect)calcVramRect
{
    if (self.view.window.contentView.frame.size.width) {
        self.view.frame = self.view.window.contentView.frame;
    } else {
        self.view.frame = CGRectMake(0, 0, 640, 480);
    }
    if (self.view.frame.size.height < self.view.frame.size.width) {
        CGFloat height = self.view.frame.size.height;
        CGFloat width = height / 3 * 4;
        CGFloat x = (self.view.frame.size.width - width) / 2;
        return NSRectFromCGRect(CGRectMake(x, 0, width, height));
    } else {
        CGFloat width = self.view.frame.size.width;
        CGFloat height = width / 4 * 3;
        CGFloat y = (self.view.frame.size.height - height) / 2;
        return NSRectFromCGRect(CGRectMake(0, y, width, height));
    }
}

- (void)viewWillAppear
{
    [self.view.window makeFirstResponder:self.vram];
}

- (void)disableMouse
{
    CGPoint centerPoint = CGPointMake(self.view.window.screen.frame.size.width / 2, self.view.window.screen.frame.size.height / 2);
    CGWarpMouseCursorPosition(centerPoint);
    CGAssociateMouseAndMouseCursorPosition(FALSE);
    CGDisplayHideCursor(kCGDirectMainDisplay);
}

- (void)viewDidAppear
{
    self.view.window.title = @"VGS Keyboard";
}

- (void)viewWillDisappear
{
    [self.vram releaseDisplayLink];
}

- (void)setRepresentedObject:(id)representedObject
{
    [super setRepresentedObject:representedObject];
}


@end
