//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#define WBVAL(x) (x & 0xFF),((x >> 8) & 0xFF)

/* USB Standard Device Descriptor */
const uint8 BulkDeviceDescriptor[] =
  {
    0x12,   /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    0x00,
    0x02,   /* bcdUSB = 2.00 */
    0x00,   /* bDeviceClass: Bulk */
    0x00,   /* bDeviceSubClass */
    0x00,   /* bDeviceProtocol */
    USB_MAX_PACKET0,           /* bMaxPacketSize0 */
    WBVAL(0x2742),             /* idVendor = 0x2742 */
    WBVAL(CMT_USB_DEV_BULK),   /* idProduct = CMT_USB_DEV_BULK */
    0x00,
    0x02,   /* bcdDevice = 2.00 */
    1,      /* Index of string descriptor describing manufacturer */
    2,      /* Index of string descriptor describing product */
    3,      /* Index of string descriptor describing the device's serial number */
    0x01    /* bNumConfigurations */
  };

#define BULK_SIZ_CONFIG_DESC        32

const uint8 BulkConfigDescriptor[] = {

  /*Configuration Descriptor*/
    0x09,   /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
    BULK_SIZ_CONFIG_DESC,       /* wTotalLength:no of returned bytes */
    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: Configuration value */
    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
    0xC0,   /* bmAttributes: self powered */
    0x32,   /* MaxPower 0 mA */
  
  /*Interface Descriptor*/
    0x09,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: 2 endpoints used */
    0xFF,   /* bInterfaceClass: no class */
    0x00,   /* bInterfaceSubClass: no subclass */
    0xff,   /* bInterfaceProtocol: no protocol */
    0x00,   /* iInterface: */

  /*Endpoint 3 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x03,   /* bEndpointAddress: (OUT3) */
    0x02,   /* bmAttributes: Bulk */
    64,     /* wMaxPacketSize: */
    0x00,
    0x00,   /* bInterval: ignore for Bulk transfer */
  
  /*Endpoint 1 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x81,   /* bEndpointAddress: (IN1) */
    0x02,   /* bmAttributes: Bulk */
    64,     /* wMaxPacketSize: */
    0x00,
    0x00,    /* bInterval */
    0x00     //stop
  };

/* USB String Descriptors */
const uint8 BulkStringLangID[4] =
  {
    4,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04 /* LangID = 0x0409: U.S. English */
  };

const uint8 BulkStringVendor[24] =
  {
    24,     /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType*/
    /* Manufacturer: "RDM-contact" */
    'R', 0, 'D', 0, 'M', 0, '-', 0, 'c', 0, 'o', 0, 'n', 0, 't', 0,
    'a', 0, 'c', 0, 't', 0
  };

const uint8 BulkStringProduct[22] =
  {
    22,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    /* Product name: "RDM-C Bulk" */
    'R', 0, 'D', 0, 'M', 0, '-', 0, 'C', 0, ' ', 0, 'B', 0, 'u', 0,
    'l', 0, 'k', 0
  };

uint8 BulkStringSerial[16] =
  {
    16,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    'R', 0, 'D', 0, 'M', 0, '-', 0, 'C', 0, '0', 0, '1', 0
  };

uint8* 
CmtUsbDevBulk::GetDeviceDescriptor() {
  return (uint8*)BulkDeviceDescriptor;
  }

uint8* 
CmtUsbDevBulk::GetConfigDescriptor( uint_8 ) {
  return (uint8*)BulkConfigDescriptor;
  }

uint8* 
CmtUsbDevBulk::GetStringDescriptor( uint_8 stringIndex ) {
  switch( stringIndex ) {
    case 1 : return (uint8*)BulkStringVendor;
    case 2 : return (uint8*)BulkStringProduct;
    case 3 : return (uint8*)BulkStringSerial;
    }
  return (uint8*)BulkStringLangID;
  }

CmtUsbDevBulk::CmtUsbDevBulk() :
  CmtUsbDev() {
  
  }
