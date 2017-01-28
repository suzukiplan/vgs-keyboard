//
//  ViewController.m
//  VGSKeyboard
//
//  Created by Yoji Suzuki on 2017/01/28.
//  Copyright © 2017年 SUZUKI PLAN. All rights reserved.
//

#import "ViewController.h"
#import "MIKMIDI.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    NSArray* availableMIDIDevices = [[MIKMIDIDeviceManager sharedDeviceManager] availableDevices];
    NSLog(@"availableMIDIDevices: %@", availableMIDIDevices);
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
