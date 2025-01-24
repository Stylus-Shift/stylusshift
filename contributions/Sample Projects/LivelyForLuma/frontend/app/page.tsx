import { Box, chakra, Container } from "@chakra-ui/react";
import { SignIn } from "@/components/SignIn";


import { Image  } from "@chakra-ui/react";
import 'mapbox-gl/dist/mapbox-gl.css';
import mapboxgl from 'mapbox-gl'

export default function Home() {
  return (
    <Container maxW="100vw" h="100vh" bg="gray.50" centerContent py={8}>
      <Box
        w={["100%", "580px"]}
        h={["100%"]}
        bg="white"
        borderRadius={["0", "40px"]}
        boxShadow="2xl"
        overflow="hidden"
        position="relative"
        border={["none", "8px solid"]}
        borderColor="gray.300"
        display="flex"
        flexDirection="column"
      >
        <Box display="flex" justifyContent="center" mb={0} pb={0}>
          <Image src="/lively.png" alt="notch"  height={35} margin={4} mb={2} pb={0} />
        </Box>

        <Box
          h="100%"
          overflowY="auto"
          px={4}
          flex="1"
          display="flex"
          flexDirection="column"
          justifyContent="space-between"
        >
          <SignIn />
        </Box>
      </Box>
    </Container>
  );
}
