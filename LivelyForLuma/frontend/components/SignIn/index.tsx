"use client";
import { signIn, signOut, useSession } from "next-auth/react";
import {
  Button,
  Image,
  Select,
  Tooltip,
  Progress,
  useDisclosure,
  Modal,
  ModalOverlay,
  ModalContent,
  ModalHeader,
  ModalFooter,
  ModalBody,
  ModalCloseButton,
  RadioGroup,
  Stack,
  Radio,
  Box,
} from "@chakra-ui/react";
import { DynamicWidget } from "@dynamic-labs/sdk-react-core";
import { useAccount } from "wagmi";
import mapboxgl from "mapbox-gl";
import {
  useReadContract,
  useWriteContract,
  useWaitForTransactionReceipt,
} from "wagmi";
import "mapbox-gl/dist/mapbox-gl.css";
import { useEffect, useRef, useState } from "react";
import abi from "../../public/abi";
import { zircuitTestnet } from "viem/chains";

export const SignIn = () => {
  const { address, isConnected, chain } = useAccount();
  const { data: session } = useSession();
  const mapRef = useRef<mapboxgl.Map | null>(null);
  const mapContainerRef = useRef<HTMLDivElement | null>(null);
  let livelyAddress: `0x${string}`;
  const livelyZAddress = process.env.NEXT_PUBLIC_LIVELYZ as `0x${string}`;
  const [sortCategory, setSortCategory] = useState("overall");

  let NEXT_PUBLIC_LIVELY="0x2B911C14C94cD3628FA6312Da70Fa706284C631B" as `0x${string}`
  let NEXT_PUBLIC_LIVELYZ="0xf83e6AF69B226d9446fB8C17CA9f258b91F0202D" as `0x${string}`

  console.log("LivelyZAddress", livelyZAddress);
  if (chain?.id === zircuitTestnet.id) {
    livelyAddress = NEXT_PUBLIC_LIVELYZ;
    
  } else {
    livelyAddress = NEXT_PUBLIC_LIVELY;
  }

  console.log(livelyAddress);

  const { data: data } = useReadContract({
    address: livelyAddress,
    abi: abi,
    functionName: "getAllEvents",
  });

  console.log("Data incoming")
  console.log(data);

  const { isOpen, onOpen, onClose } = useDisclosure();
  const [selectedEventIndex, setSelectedEventIndex] = useState<number | null>(
    null
  );
  const [overallVote, setOverallVote] = useState("0");
  const [foodVote, setFoodVote] = useState("0");
  const [technicalVote, setTechnicalVote] = useState("0");
  const [networkingVote, setNetworkingVote] = useState("0");
  const [funVote, setFunVote] = useState("0");
  const [swagVote, setSwagVote] = useState("0");
  const [originalIndices, setOriginalIndices] = useState<number[]>([]);

  const { data: hash, isPending, writeContract, error } = useWriteContract();

  const handleSubmit = async () => {
    if (selectedEventIndex !== null) {
      console.log({
        selectedEventIndex,
        overallVote,
        foodVote,
        technicalVote,
        networkingVote,
        funVote,
        swagVote,
      });
      writeContract({
        address: livelyAddress, // Replace with your contract address
        abi,
        functionName: "rateEvent",
        args: [
          BigInt(selectedEventIndex),
          parseInt(overallVote),
          parseInt(foodVote),
          parseInt(technicalVote),
          parseInt(networkingVote),
          parseInt(funVote),
          parseInt(swagVote),
        ],
      });
    }
  };

  const { isLoading: isConfirming, isSuccess: isConfirmed } =
    useWaitForTransactionReceipt({
      hash,
    });

  useEffect(() => {
    if (mapContainerRef.current) {
      mapboxgl.accessToken =
        "pk.eyJ1IjoidGFjb2NhdDQ2NDIiLCJhIjoiY20za280d2Z4MGd1azJrczcxMmFuajBueCJ9.LHliYmwenOrUXMiEtWLGxQ";

      navigator.geolocation.getCurrentPosition(
        (position) => {
          const { latitude, longitude } = position.coords;

          mapRef.current = new mapboxgl.Map({
            container: mapContainerRef.current as HTMLElement,
            style: "mapbox://styles/mapbox/streets-v12",
            center: [longitude, latitude],
            zoom: 11.12,
            dragPan: false,
          });

          mapRef.current.scrollZoom.disable();

          mapRef.current.addControl(
            new mapboxgl.GeolocateControl({
              positionOptions: {
                enableHighAccuracy: true,
              },
              trackUserLocation: true,
              showUserHeading: true,
            })
          );

          // Create a custom marker element for the user's current location
          const userLocationMarker = document.createElement("div");
          userLocationMarker.className = "user-location-marker";

          // Add the custom marker to the map
          new mapboxgl.Marker(userLocationMarker)
            .setLngLat([longitude, latitude])
            .addTo(mapRef.current);

          // Add markers if data is available
          if (data) {
            // @ts-ignore
            data.forEach((event, index) => {
              const marker = new mapboxgl.Marker()
                .setLngLat([parseFloat(event.long), parseFloat(event.lat)])
                .setPopup(
                  new mapboxgl.Popup({
                    offset: [0, -15],
                    anchor: "bottom",
                  }).setHTML(
                    `<h3>${event.name}</h3><a href="${event.lumaLink}" target="_blank" style="color: #E91E63;">View Event</a>`
                  )
                )
                .addTo(mapRef.current!);

              // Add click event to scroll to the respective event in the list
              marker.getElement().addEventListener("click", () => {
                const eventElement = document.getElementById(`event-${index}`);
                eventElement?.scrollIntoView({ behavior: "smooth" });
              });
            });
          }
        },
        (error) => {
          console.error("Error getting user's location:", error);

          mapRef.current = new mapboxgl.Map({
            container: mapContainerRef.current as HTMLElement,
            style: "mapbox://styles/mapbox/streets-v12",
            center: [-74.0242, 40.6941],
            zoom: 11.12,
            dragPan: false,
          });

          mapRef.current.scrollZoom.disable();

          mapRef.current.addControl(
            new mapboxgl.GeolocateControl({
              positionOptions: {
                enableHighAccuracy: true,
              },
              trackUserLocation: true,
              showUserHeading: true,
            })
          );

          // Add markers if data is available (in error callback)
          if (data) {
            data.forEach((event, index) => {
              const marker = new mapboxgl.Marker()
                .setLngLat([parseFloat(event.long), parseFloat(event.lat)])
                .setPopup(
                  new mapboxgl.Popup({
                    offset: [0, -15],
                    anchor: "bottom",
                  }).setHTML(
                    `<h3>${event.name}</h3><a href="${event.lumaLink}" target="_blank" style="color: #E91E63;">View Event</a>`
                  )
                )
                .addTo(mapRef.current!);

              // Add click event to scroll to the respective event in the list
              marker.getElement().addEventListener("click", () => {
                const eventElement = document.getElementById(`event-${index}`);
                eventElement?.scrollIntoView({ behavior: "smooth" });
              });
            });
          }
        }
      );

      return () => {
        mapRef.current?.remove();
      };
    }
  }, [data]);

  useEffect(() => {
    if (data) {
      // @ts-ignore
      setOriginalIndices(data.map((_, index) => index));
    }
  }, [data]);

  if (!session) {
    return (
      <Box
        width="100%"
        height="100vh"
        display="flex"
        justifyContent="center"
        alignItems="center"
      >
        <Button onClick={() => signIn("worldcoin")}>
          Sign in with Worldcoin
        </Button>
      </Box>
    );
  }

  // Check if address is available
  if (session && !address ) {
    return <DynamicWidget />; // or any placeholder component if you prefer
  }

  return (
    <>
      <Box
        display="flex"
        flexDirection="column"
        justifyContent="center"
        alignItems="center"
        width="100%"
        height="100vh"
        overflow="hidden"
      >
        {/* Dynamic Widget and Select */}
        <Box
          display="flex"
          justifyContent="space-between"
          width="100%"
          padding="4"
        >
          <DynamicWidget />
          <Select onChange={(e) => setSortCategory(e.target.value)}>
            <option value="overall">Overall</option>
            <option value="food">Food</option>
            <option value="technical">Technical</option>
            <option value="networking">Networking</option>
            <option value="swag">Swag</option>
          </Select>
        </Box>

        {/* Event List */}
        <Box overflowY="auto" width="100%" height="100%" margin="4" px="0">
          {data &&
            data
              // @ts-ignore: Suppress implicit 'any' type error
              .map((event, index) => ({
                ...event,
                originalIndex: originalIndices[index],
              }))
              .sort((a, b) => {
                switch (sortCategory) {
                  case "food":
                    return b.stats.food - a.stats.food;
                  case "technical":
                    return b.stats.technical - a.stats.technical;
                  case "networking":
                    return b.stats.networking - a.stats.networking;
                  case "swag":
                    return b.stats.swag - a.stats.swag;
                  case "overall":
                  default:
                    return b.stats.overall - a.stats.overall;
                }
              })
              .map((event, displayIndex) => (
                <Tooltip
                  label={
                    <Box>
                      <Box>
                        Food:{" "}
                        <Progress
                          value={event.stats.food}
                          max={10}
                          size="xs"
                          colorScheme="pink"
                        />{" "}
                        {event.stats.food}
                      </Box>
                      <Box>
                        Technical:{" "}
                        <Progress
                          value={event.stats.technical}
                          max={10}
                          size="xs"
                          colorScheme="blue"
                        />{" "}
                        {event.stats.technical}
                      </Box>
                      <Box>
                        Networking:{" "}
                        <Progress
                          value={event.stats.networking}
                          max={10}
                          size="xs"
                          colorScheme="green"
                        />{" "}
                        {event.stats.networking}
                      </Box>
                      <Box>
                        Swag:{" "}
                        <Progress
                          value={event.stats.swag}
                          max={10}
                          size="xs"
                          colorScheme="purple"
                        />{" "}
                        {event.stats.swag}
                      </Box>
                      <Box>
                        Overall:{" "}
                        <Progress
                          value={event.stats.overall}
                          max={10}
                          size="xs"
                          colorScheme="orange"
                        />{" "}
                        {event.stats.overall}
                      </Box>
                    </Box>
                  }
                  placement="top"
                  hasArrow
                  bg="white"
                  color="black"
                  borderRadius="md"
                  p="2"
                >
                  <Box
                    id={`event-${displayIndex}`}
                    // @ts-ignore
                    key={event.id}
                    display="flex"
                    flexDirection="row"
                    alignItems="center"
                    borderWidth="1px"
                    borderRadius="lg"
                    overflow="hidden"
                    marginBottom="4"
                    padding="4"
                    onClick={() => window.open(event.lumaLink, "_blank")}
                    cursor="pointer"
                  >
                    <Box flexShrink="0">
                      <Image
                        src={event.imageUrl}
                        alt={event.name}
                        width="100px"
                      />
                    </Box>
                    <Box marginLeft="4" width="100%">
                      <Box
                        fontWeight="bold"
                        as="h3"
                        marginBottom="2"
                        width="100%"
                      >
                        {displayIndex + 1}. {event.name}
                      </Box>
                      <Box
                        display="flex"
                        justifyContent="space-between"
                        color="gray.500"
                        fontSize="sm"
                        marginBottom="2"
                      >
                        <Box>{event.venue}</Box>
                        <Box marginLeft="auto" paddingLeft="2">
                          {event.time}
                        </Box>
                      </Box>
                      <Button
                        onClick={(e) => {
                          e.stopPropagation();
                          setSelectedEventIndex(event.originalIndex);
                          onOpen();
                        }}
                      >
                        Rate
                      </Button>
                    </Box>
                  </Box>
                </Tooltip>
              ))}
        </Box>

        {/* Map Container */}
        <Box
          id="map-container"
          ref={mapContainerRef}
          width="100%"
          height="30%"
          borderRadius="lg"
          borderWidth="1px"
          margin="4"
          padding="4"
          overflow="hidden"
        />
      </Box>

      {/* Rating Modal */}
      <Modal isOpen={isOpen} onClose={onClose}>
        <ModalOverlay />
        <ModalContent>
          <ModalHeader>Rate Event</ModalHeader>
          <ModalCloseButton />
          <ModalBody>
            <RadioGroup onChange={setOverallVote} value={overallVote}>
              <Stack direction="column">
                <Box>
                  Overall:
                  <Radio value="1">Upvote</Radio>
                  <Radio value="2">Downvote</Radio>
                  <Radio value="0">No Vote</Radio>
                </Box>
              </Stack>
            </RadioGroup>
            <RadioGroup onChange={setFoodVote} value={foodVote}>
              <Stack direction="column">
                <Box>
                  Food:
                  <Radio value="1">Upvote</Radio>
                  <Radio value="2">Downvote</Radio>
                  <Radio value="0">No Vote</Radio>
                </Box>
              </Stack>
            </RadioGroup>
            <RadioGroup onChange={setTechnicalVote} value={technicalVote}>
              <Stack direction="column">
                <Box>
                  Technical:
                  <Radio value="1">Upvote</Radio>
                  <Radio value="2">Downvote</Radio>
                  <Radio value="0">No Vote</Radio>
                </Box>
              </Stack>
            </RadioGroup>
            <RadioGroup onChange={setNetworkingVote} value={networkingVote}>
              <Stack direction="column">
                <Box>
                  Networking:
                  <Radio value="1">Upvote</Radio>
                  <Radio value="2">Downvote</Radio>
                  <Radio value="0">No Vote</Radio>
                </Box>
              </Stack>
            </RadioGroup>
            <RadioGroup onChange={setFunVote} value={funVote}>
              <Stack direction="column">
                <Box>
                  Fun:
                  <Radio value="1">Upvote</Radio>
                  <Radio value="2">Downvote</Radio>
                  <Radio value="0">No Vote</Radio>
                </Box>
              </Stack>
            </RadioGroup>
            <RadioGroup onChange={setSwagVote} value={swagVote}>
              <Stack direction="column">
                <Box>
                  Swag:
                  <Radio value="1">Upvote</Radio>
                  <Radio value="2">Downvote</Radio>
                  <Radio value="0">No Vote</Radio>
                </Box>
              </Stack>
            </RadioGroup>
          </ModalBody>
          <ModalFooter>
            <Button
              colorScheme="blue"
              mr={3}
              onClick={handleSubmit}
              disabled={isPending}
            >
              {isPending ? "Submitting..." : "Submit"}
            </Button>
            {hash && <div>Transaction Hash: {hash}</div>}
            {isConfirming && <div>Waiting for confirmation...</div>}
            {isConfirmed && <div>Transaction confirmed.</div>}
            {error && <div>Error: {error.message}</div>}
          </ModalFooter>
        </ModalContent>
      </Modal>
    </>
  );
};

function SignInThingy() {
  return (
    <Box
      width="100%"
      height="100%"
      display="flex"
      justifyContent="center"
      alignItems="center"
      margin="2rem"
    >
      <Box></Box>
    </Box>
  );
}
