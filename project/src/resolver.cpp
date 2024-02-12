// IMPORT THIRD-PARTY LIBRARIES
#include <pxr/usd/ar/defaultResolver.h>
#include <pxr/usd/ar/defineResolver.h>
#include <iostream>

// IMPORT LOCAL LIBRARIES
#include "resolver.h"

PXR_NAMESPACE_OPEN_SCOPE

AR_DEFINE_RESOLVER(URIResolver, ArResolver)

URIResolver::URIResolver() : ArDefaultResolver() {}
URIResolver::~URIResolver() = default;

ArResolvedPath URIResolver::_Resolve(const std::string& assetPath) const {
    std::cout << "test" << std::endl;
    if (assetPath == "/foo") {
        return ArResolvedPath("/bar");
    }

    return ArResolvedPath("/yo");
}

PXR_NAMESPACE_CLOSE_SCOPE
